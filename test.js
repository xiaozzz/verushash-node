var cluster = require('cluster');
var vh = require('bindings')('verushash.node');

var reverseBuffer = function (buff) {
    var reversed = Buffer.alloc(buff.length);
    for (var i = buff.length - 1; i >= 0; i--)
        reversed[buff.length - i - 1] = buff[i];
    return reversed;
};
var reverseHex = function (hex) {
    return reverseBuffer(Buffer.from(hex, 'hex')).toString('hex');
};

var numWorkers = require('os').cpus().length;
numWorkers = 20;

if (cluster.isMaster) {
    
    var workers = [];
    var gbtCount = 0;
    for (var i = 0; i < numWorkers; i++){
        var worker = cluster.fork({
            workerType: 'VerusHasher',
            forkId: i
        });
        workers.push(worker);
    }
    
} else {
    
    var output = vh.hash(Buffer.from('Test1234','utf8'));
    console.log(process.pid,'VerusHash1 Output', reverseHex(output.toString('hex')), '\n');    
    output = vh.init().update(Buffer.from('Test','utf8')).update(Buffer.from('123','utf8')).update(Buffer.from('4','utf8')).digest();
    console.log(process.pid,'VerusHash1 Output', reverseHex(output.toString('hex')), '\n');

    output = vh.hash2(Buffer.from('Test1234','utf8'));
    console.log(process.pid,'VerusHash2 Output', reverseHex(output.toString('hex')), '\n');    
    output = vh.init().update2(Buffer.from('Test','utf8')).update2(Buffer.from('123','utf8')).update2(Buffer.from('4','utf8')).digest2();
    console.log(process.pid,'VerusHash2 Output', reverseHex(output.toString('hex')), '\n');
}
