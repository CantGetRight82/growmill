

const fs = require('fs');
const cp = require('child_process');

var lines = cp
	.execSync('cd example && ls *.mil')
	.toString()
	.trim()
	.split("\n");

if(!fs.existsSync('output')) {
	fs.mkdirSync('output');
}

lines.forEach( (file) => {
	var ifile = 'example/'+file;
	var ofile = 'output/'+file.substr(0,file.length-3) + 'svg';
	cp.execSync('./Debug/growc '+ifile+' '+ofile+' 100 100');
	// con
});
