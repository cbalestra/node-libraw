var fs = require('fs');
var expect = require('chai').expect;
var libraw = require('./libraw');
const request = require('request')

let url = 'https://content.protriathletes.org/media/content/test/032ea20c-57db-425e-9df3-f0e4b4eeee12.NEF'
url = 'https://content.protriathletes.org/media/content/test/1e56cbf7-a17d-4736-8b66-a2d80049791f.NEF'

/*let t = new Uint8Array([10, 20, 30, 40, 50])
var buffer = new ArrayBuffer(8) //)
console.log("--", t.buffer)
libraw.extract2(buffer)
.then(() => {
	console.log("done")
})*/

request({ url: url, encoding: null }, async (error, __response, body) => {
	console.log(body)
	libraw.extractFromMemory(body.buffer)
	.then((tt) => {
		console.log("done", tt)
		fs.writeFile('test.jpg', tt.data, null, () => {
			console.log("file written")
		})
	})
})


/*describe('LibRAW', function() {
  describe('Extracting thumbnail', function () {
    var outputFilename = '';

    it('should extract the thumbnail', function () {
      libraw.extractThumb('./test.raf', './output')
        .then(function(output) {
          expect(output).to.equal('./output.thumb.jpg');
          outputFilename = output;
        });
    });

    it('should check that the file has been properly created', function(done) {
      fs.access(outputFilename, fs.F_OK, function(err) {
        expect(err).to.be.null;
        done();
      });
    });

    it('should delete the file after creation', function(done) {
      fs.unlink(outputFilename, done);
    });
  });

  describe('Extracting TIFF', function () {
    var outputFilename = '';

    it('should extract the tiff', function () {
      this.timeout(90000);

      libraw.extract('./test.raf', './output')
        .then(function(output) {
          expect(output).to.equal('./output.tiff');
          outputFilename = output;
        });
    });

    it('should check that the file has been properly created', function(done) {
      fs.access(outputFilename, fs.F_OK, function(err) {
        expect(err).to.be.null;
        done();
      });
    });

    it('should delete the file after creation', function(done) {
      fs.unlink(outputFilename, done);
    });
  });

});
*/