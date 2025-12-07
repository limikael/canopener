import ctor from "../dist/cof-wasm.js";
import defines from "../dist/cof-defines.js";

let COF=await ctor();

COF.slcanStringify=cof=>{
	let p=COF._malloc(256);
	COF._cof_to_slcan(cof,p);

	let res="";
	for (let i=p; COF.HEAPU8[i]; i++)
		res+=String.fromCharCode(COF.HEAPU8[i]);

	COF._free(p);
	return res;
}

COF.slcanParse=slcan=>{
	let s=COF._malloc(256);
	//console.log("s="+s);
	for (let i=0; i<slcan.length; i++)
		COF.HEAPU8[s+i]=slcan[i].charCodeAt(0);

	COF.HEAPU8[s+slcan.length]=0;

	let cof=COF._cof_create();
	let res=COF._cof_from_slcan(cof,s);
	//console.log("res= ",res);
	COF._free(s);

	return cof;
}

COF={...COF,...defines};

export default COF;
