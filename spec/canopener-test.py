import can
import canopen
import logging
from pathlib import Path
import asyncio

dirname=Path(__file__).resolve().parent
#logging.basicConfig(level=logging.DEBUG)

class Emitter(can.Listener):
    def on_message_received(self, msg):
        #print(f"ID=0x{msg.arbitration_id:03X} ",f"DATA={msg.data.hex(' ')}")
        emit("message",msg.arbitration_id,",".join(str(b) for b in msg.data))

connector=canopen.Network()
connector.NOTIFIER_SHUTDOWN_TIMEOUT = 0.0
connector.connect("test",interface="virtual")
connector.notifier.add_listener(Emitter())

async def message(id, text):
	data=[int(x) for x in text.split(",")]
	msg=can.Message(arbitration_id=id, data=data, is_extended_id=False)
	connector.bus.send(msg)

async def sleep():
    print("before")
    await asyncio.sleep(2)
    print("after")	

async def read_sdo(node, index):
    return await asyncio.to_thread(
        lambda: node.sdo[index].raw
    )

async def write_sdo(node, index, val):
    def do_write():
        node.sdo[index].raw = val

    return await asyncio.to_thread(do_write)

async def test_sdo_read():
	bus=canopen.Network()
	bus.NOTIFIER_SHUTDOWN_TIMEOUT = 0.0
	bus.connect("test",interface="virtual")
	node=canopen.RemoteNode(6,str(dirname/"dummy.eds"))
	node.sdo.RESPONSE_TIMEOUT=10.0
	bus.add_node(node)
	val=await read_sdo(node,0x2000)
	return val

async def test_sdo_write():
	bus=canopen.Network()
	bus.NOTIFIER_SHUTDOWN_TIMEOUT = 0.0
	bus.connect("test",interface="virtual")
	node=canopen.RemoteNode(6,str(dirname/"dummy.eds"))
	node.sdo.RESPONSE_TIMEOUT=10.0
	bus.add_node(node)
	val=await write_sdo(node,0x2000,555)
	return val
