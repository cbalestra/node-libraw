#include <stdio.h>
#include <iostream>
#include <fstream>
#include <v8.h>
#include <nan.h>
//#include <libraw/libraw.h>

#include "libraw/libraw.h"

namespace node_libraw {
  using v8::Exception;
  using v8::Function;
  using v8::FunctionCallbackInfo;
  using v8::FunctionTemplate;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using v8::Null;
  using v8::ArrayBuffer;

  NAN_METHOD(Extract) {
    Nan::HandleScope scope;

    LibRaw RawProcessor;

    v8::Isolate* isolate = info.GetIsolate();
    v8::String::Utf8Value filenameFromArgs(isolate, info[0]);
    std::string filename = std::string(*filenameFromArgs);
    v8::String::Utf8Value outputFromArgs(isolate, info[1]);
    std::string output = std::string(*outputFromArgs);

    Nan::Callback *callback = new Nan::Callback(Local<Function>::Cast(info[2]));

    std::ifstream file;
    file.open(filename.c_str(), std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);

    if (file.read(buffer.data(), size)) {
      RawProcessor.open_buffer(buffer.data(), size);
      RawProcessor.unpack();
      RawProcessor.imgdata.params.output_tiff = 1;
      RawProcessor.imgdata.params.output_bps = 16;
      RawProcessor.imgdata.params.use_camera_wb = 1;
      RawProcessor.dcraw_process();

      output = output + ".tiff";
      RawProcessor.dcraw_ppm_tiff_writer(output.c_str());
      RawProcessor.recycle();

      //info.GetReturnValue().Set(Nan::New(output).ToLocalChecked());

      Local<v8::Value> argv[2] = {
        Nan::Null(),
        Nan::New(output).ToLocalChecked()
      };

      callback->Call(2, argv);
    }

    file.close();
  }
  	// https://medium.com/@muehler.v/tutorial-to-node-js-native-c-modules-part-2-arrays-json-and-callbacks-9b81f09874cd

	NAN_METHOD(ExtractFromMemory) {
		Nan::HandleScope scope;
		LibRaw RawProcessor;

		Nan::Callback *callback = new Nan::Callback(Local<Function>::Cast(info[2]));

		v8::Local<v8::ArrayBuffer> input = v8::Local<v8::ArrayBuffer>::Cast(info[0]);
		printf("%zu\n", input->ByteLength());
		/*v8::Isolate* isolate = info.GetIsolate();
		Local<v8::ArrayBuffer> bufferInput = v8::ArrayBuffer::Cast(info[0]);
		*/
		//Local<Object> bufferObj = info[0]->ToObject();
		//unsigned char*buf = (unsigned char*) node::Buffer::Data(info[0]->ToObject());

		RawProcessor.open_buffer(input->GetContents().Data(), input->ByteLength());
		RawProcessor.unpack();
		/*RawProcessor.imgdata.params.output_tiff = 1;
		RawProcessor.imgdata.params.output_bps = 16;
		RawProcessor.imgdata.params.use_camera_wb = 1;
		RawProcessor.dcraw_process();*/

		RawProcessor.unpack_thumb();



		//printf("%d, %d, %p\n", RawProcessor.imgdata.thumbnail.twidth, RawProcessor.imgdata.thumbnail.theight, RawProcessor.imgdata.thumbnail.thumb);

		//output = output + ".tiff";
		//RawProcessor.dcraw_ppm_tiff_writer(output.c_str());

		v8::Local<v8::Object> jsonObject = Nan::New<v8::Object>();

		v8::Local<v8::String> widthProp = Nan::New("width").ToLocalChecked();
		v8::Local<v8::String> heightProp = Nan::New("height").ToLocalChecked();
		v8::Local<v8::String> sizeProp = Nan::New("size").ToLocalChecked();
		v8::Local<v8::String> flipProp = Nan::New("flip").ToLocalChecked();
		v8::Local<v8::Value> widthValue = Nan::New(RawProcessor.imgdata.thumbnail.twidth);
		v8::Local<v8::Value> heightValue = Nan::New(RawProcessor.imgdata.thumbnail.theight);
		v8::Local<v8::Value> sizeValue = Nan::New(RawProcessor.imgdata.thumbnail.tlength);
		v8::Local<v8::Value> flipValue = Nan::New(RawProcessor.imgdata.sizes.flip);
		Nan::Set(jsonObject, widthProp, widthValue);
		Nan::Set(jsonObject, heightProp, heightValue);
		Nan::Set(jsonObject, sizeProp, sizeValue);
		Nan::Set(jsonObject, flipProp, flipValue);

		v8::Local<v8::String> dataProp = Nan::New("data").ToLocalChecked();
		//v8::Local<v8::Buffer> dataValue =

		Nan::Set(jsonObject, dataProp,  Nan::CopyBuffer(RawProcessor.imgdata.thumbnail.thumb, RawProcessor.imgdata.thumbnail.tlength).ToLocalChecked());

		//v8::Local<v8::ArrayBuffer> arr = Nan::New<v8::ArrayBuffer>(info.GetIsolate(), (void*)RawProcessor.imgdata.thumbnail.thumb, (size_t)RawProcessor.imgdata.thumbnail.tlength);

		Local<v8::Value> argv[2] = {
			Nan::Null(),
			jsonObject
			//Nan::New(arr).ToLocalChecked()
			//Nan::New(output).ToLocalChecked()
		};
		callback->Call(2, argv);

		RawProcessor.recycle();

    /*v8::Isolate* isolate = info.GetIsolate();
    v8::String::Utf8Value filenameFromArgs(isolate, info[0]);
    std::string filename = std::string(*filenameFromArgs);
    v8::String::Utf8Value outputFromArgs(isolate, info[1]);
    std::string output = std::string(*outputFromArgs);

    Nan::Callback *callback = new Nan::Callback(Local<Function>::Cast(info[2]));

    std::ifstream file;
    file.open(filename.c_str(), std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);

    if (file.read(buffer.data(), size)) {
      RawProcessor.open_buffer(buffer.data(), size);
      RawProcessor.unpack();
      RawProcessor.imgdata.params.output_tiff = 1;
      RawProcessor.imgdata.params.output_bps = 16;
      RawProcessor.imgdata.params.use_camera_wb = 1;
      RawProcessor.dcraw_process();

      output = output + ".tiff";
      RawProcessor.dcraw_ppm_tiff_writer(output.c_str());
      RawProcessor.recycle();

      //info.GetReturnValue().Set(Nan::New(output).ToLocalChecked());

      Local<v8::Value> argv[2] = {
        Nan::Null(),
        Nan::New(output).ToLocalChecked()
      };

      callback->Call(2, argv);
    }

    file.close();*/
  }


  NAN_METHOD(ExtractThumb) {
    Nan::HandleScope scope;

    LibRaw RawProcessor;

    v8::Isolate* isolate = info.GetIsolate();
    v8::String::Utf8Value filenameFromArgs(isolate, info[0]);
    std::string filename = std::string(*filenameFromArgs);
    v8::String::Utf8Value outputFromArgs(isolate, info[1]);
    std::string output = std::string(*outputFromArgs);

    Nan::Callback *callback = new Nan::Callback(Local<Function>::Cast(info[2]));

    std::string extension = "thumb.ppm";

    std::ifstream file;
    file.open(filename.c_str(), std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);

    if (file.read(buffer.data(), size)) {
      RawProcessor.open_buffer(buffer.data(), size);
      RawProcessor.unpack();
      RawProcessor.unpack_thumb();

      if (RawProcessor.imgdata.thumbnail.tformat == LIBRAW_THUMBNAIL_JPEG) {
        extension = "thumb.jpg";
      }

      output = output + "." + extension;
      RawProcessor.dcraw_thumb_writer(output.c_str());
      RawProcessor.recycle();

      Local<v8::Value> argv[2] = {
        Nan::Null(),
        Nan::New(output).ToLocalChecked()
      };

      callback->Call(2, argv);
    }

    file.close();
  }

  void init(Local<Object> exports) {
    Nan::Set(
      exports,
      Nan::New<String>("extractThumb").ToLocalChecked(),
      Nan::GetFunction(Nan::New<v8::FunctionTemplate>(ExtractThumb)).ToLocalChecked()
    );

    Nan::Set(
      exports,
      Nan::New<String>("extract").ToLocalChecked(),
      Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Extract)).ToLocalChecked()
    );

    Nan::Set(
      exports,
      Nan::New<String>("extractFromMemory").ToLocalChecked(),
      Nan::GetFunction(Nan::New<v8::FunctionTemplate>(ExtractFromMemory)).ToLocalChecked()
    );
  }

  NODE_MODULE(libraw, init)
}
