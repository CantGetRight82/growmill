

const fs = require('fs');
const cp = require('child_process');

var lines = cp
	.execSync('cd ad && ls *.yy')
	.toString()
	.trim()
	.split("\n");

if(!fs.existsSync('output')) {
	fs.mkdirSync('output');
}
// cp.execSync('rm output/*');

lines.forEach( (file) => {
	var ifile = 'ad/'+file;
	var ofile = 'output/'+file.replace('.yy', '.svg');
	cp.execSync('./Debug/growc '+ifile+' '+ofile+' 360 640');
	// con
});
