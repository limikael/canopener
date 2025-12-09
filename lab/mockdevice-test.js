import child_process from "child_process";

let proc=child_process.spawn("bin/mockdevice");
//let proc=child_process.spawn("/usr/bin/ls");
proc.on("spawn",()=>console.log("spawn..."));
proc.stdout.on("data",data=>console.log(data.toString()));

await new Promise(r=>setTimeout(r,10000));