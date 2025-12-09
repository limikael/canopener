import {SerialPort} from "serialport";
import {ReadlineParser} from '@serialport/parser-readline'
import EventEmitter from "node:events";
import child_process from "child_process";

export function slcanParse(s) {
    s = s.trim();
    if (s.length === 0) return undefined;
    
    const type = s[0];
    
    // Only handle t, T, r, R
    if (!['t', 'T', 'r', 'R'].includes(type)) {
        return undefined;
    }
    
    const isExtended = type === 'T' || type === 'R';
    const isRTR = type === 'r' || type === 'R';
    
    if (isExtended) {
        // Extended frame: TiiiiiiiiLdd... or RiiiiiiiiL
        if (s.length < 10) return undefined;
        
        const idStr = s.substring(1, 9);
        const id = parseInt(idStr, 16);
        if (isNaN(id)) return undefined;
        
        const len = parseInt(s[9], 16);
        if (isNaN(len) || len < 0 || len > 8) return undefined;
        
        if (isRTR) {
            // RTR frame: RiiiiiiiiL
            if (s.length !== 10) return undefined; // Just ID + length, no data
            return {
                id: id,
                data: Buffer.alloc(0),
                ext: true,
                rtr: true
            };
        } else {
            // Data frame: TiiiiiiiiLdd...
            const expectedLength = 10 + len * 2;
            if (s.length !== expectedLength) return undefined;
            
            const dataStr = s.substring(10, 10 + len * 2);
            const data = Buffer.from(dataStr.match(/.{1,2}/g).map(b => parseInt(b, 16)));
            
            return {
                id: id,
                data: data,
                ext: true,
                rtr: false
            };
        }
    } else {
        // Standard frame: tiiiLdd... or riiiL
        if (s.length < 5) return undefined;
        
        const idStr = s.substring(1, 4);
        const id = parseInt(idStr, 16);
        if (isNaN(id)) return undefined;
        
        const len = parseInt(s[4], 16);
        if (isNaN(len) || len < 0 || len > 8) return undefined;
        
        if (isRTR) {
            // RTR frame: riiiL
            if (s.length !== 5) return undefined; // Just ID + length, no data
            return {
                id: id,
                data: Buffer.alloc(0),
                ext: false,
                rtr: true
            };
        } else {
            // Data frame: tiiiLdd...
            const expectedLength = 5 + len * 2;
            if (s.length !== expectedLength) return undefined;
            
            const dataStr = s.substring(5, 5 + len * 2);
            const data = Buffer.from(dataStr.match(/.{1,2}/g).map(b => parseInt(b, 16)));
            
            return {
                id: id,
                data: data,
                ext: false,
                rtr: false
            };
        }
    }
}

export function slcanStringify(frame) {
    // Validate required properties
    if (!frame || typeof frame.id !== 'number') {
        return undefined;
    }
    
    const id = frame.id;
    const data = frame.data || Buffer.alloc(0);
    const isExtended = frame.ext || false;
    const isRTR = frame.rtr || false;
    
    // Validate data is Buffer-like
    let dataLength = 0;
    let dataBytes = [];
    
    if (data && !isRTR) {
        if (Buffer.isBuffer(data)) {
            dataLength = Math.min(data.length, 8);
            dataBytes = Array.from(data.slice(0, dataLength));
        } else if (Array.isArray(data) || ArrayBuffer.isView(data)) {
            dataLength = Math.min(data.length, 8);
            dataBytes = Array.from(data).slice(0, dataLength);
        } else {
            return undefined;
        }
    }
    
    // Determine frame type character
    let typeChar;
    if (isRTR) {
        typeChar = isExtended ? 'R' : 'r';
    } else {
        typeChar = isExtended ? 'T' : 't';
    }
    
    // Format ID
    let idStr;
    if (isExtended) {
        // Extended ID: 8 hex digits, zero-padded
        idStr = id.toString(16).padStart(8, '0').toUpperCase().slice(-8);
    } else {
        // Standard ID: 3 hex digits, zero-padded
        idStr = id.toString(16).padStart(3, '0').toUpperCase().slice(-3);
    }
    
    // Format data length
    const lenStr = dataLength.toString(16).toUpperCase();
    
    // Format data bytes
    let dataStr = '';
    if (!isRTR && dataLength > 0) {
        dataStr = dataBytes.map(byte => 
            byte.toString(16).padStart(2, '0').toUpperCase()
        ).join('');
    }
    
    // Build SLCAN string
    const slcanStr = typeChar + idStr + lenStr + dataStr;
    return slcanStr;
}

export default class SlcanBus extends EventEmitter {
	constructor(options) {
		super();

        if (options.spawn) {
            this.proc=child_process.spawn("bin/mockdevice");
            this.readStream=this.proc.stdout;
            this.writeStream=this.proc.stdin;
            this.proc.on("spawn",()=>this.emit("open"));
            this.proc.on("error",()=>console.log("Error!!!"));
        }

        else {
            this.port=new SerialPort(options);
            this.readStream=this.port;
            this.writeStream=this.port;
            this.port.on("open",()=>this.emit("open"));
        }

        this.parser=this.readStream.pipe(new ReadlineParser(/*{delimiter: ...}*/));
        this.parser.on("data",this.handleData);

	}

	send(frame) {
        if (typeof frame!="string")
    		frame=slcanStringify(frame);

		if (!frame)
			throw new Error("Not a CAN frame!");

        //console.log("send in bus:",frame);
        this.writeStream.write(frame+"\r\n");
	}

	handleData=data=>{
        try {
            //console.log("Raw: ",data.toString());

            let frame=slcanParse(data.toString());
            //console.log("frame: ",frame);
            if (frame)
                this.emit("message",frame);

            else
                console.log("# ",data.toString());
        }

        catch (e) {
            console.log(e);
        }
	}
}

export function openSlcanBus(options) {
	return new Promise((resolve,reject)=>{
		let bus=new SlcanBus(options);
		bus.on("open",()=>resolve(bus));
		bus.on("error",reject);
	});
}
