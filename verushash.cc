#include <nan.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdint.h>
#include <vector>

#include "crypto/verus_hash.h"

using namespace v8;

CVerusHash* vh;
bool initialized = false;

void verusInit(const v8::FunctionCallbackInfo<Value>& args) {
    vh = new CVerusHash();
    vh->init();
    initialized = true;
    
    args.GetReturnValue().Set(args.This());
}

void verusUpdate(const v8::FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    if (initialized == false){
        isolate->ThrowException(
            Exception::TypeError(String::NewFromUtf8(isolate, "call init() first!"))
        );
    }
    if (args.Length() < 1) {
        isolate->ThrowException(
            Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments"))
        );
        return;
    }
    Local<Object> buffer = args[0]->ToObject();
    if(!node::Buffer::HasInstance(buffer)) {
        isolate->ThrowException(
            Exception::TypeError(String::NewFromUtf8(isolate, "Invalid buffer objects."))
        );
        return;
    }
    
    const char *buff = node::Buffer::Data(buffer);
    vh->Write((const unsigned char *)buff, node::Buffer::Length(buffer));
    
    args.GetReturnValue().Set(args.This());
}

void verusDigest(const v8::FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    if (initialized == false){
        isolate->ThrowException(
            Exception::TypeError(String::NewFromUtf8(isolate, "call init() first!"))
        );
    }
    char *result = new char[32];
    vh->Finalize((unsigned char *)result);
    args.GetReturnValue().Set(Nan::NewBuffer(result, 32).ToLocalChecked());
}

void verusReset(const v8::FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    if (initialized == false){
        isolate->ThrowException(
            Exception::TypeError(String::NewFromUtf8(isolate, "call init() first!"))
        );
    }
    vh->Reset();
    args.GetReturnValue().Set(args.This());
}

void verusHash(const v8::FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    if (args.Length() < 1) {
        isolate->ThrowException(
            Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments"))
        );
        return;
    }
    Local<Object> buffer = args[0]->ToObject();
    if(!node::Buffer::HasInstance(buffer)) {
        isolate->ThrowException(
            Exception::TypeError(String::NewFromUtf8(isolate, "Invalid buffer objects."))
        );
        return;
    }

    const char *buff = node::Buffer::Data(buffer);

    char *result = new char[32];
    
    if (initialized == false) {
        CVerusHash::init();
        initialized = true;
    }
    verus_hash(result, buff, node::Buffer::Length(buffer));
    
    args.GetReturnValue().Set(Nan::NewBuffer(result, 32).ToLocalChecked());
}


void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "init", verusInit);
  NODE_SET_METHOD(exports, "update", verusUpdate);
  NODE_SET_METHOD(exports, "digest", verusDigest);
  NODE_SET_METHOD(exports, "reset", verusReset);
  NODE_SET_METHOD(exports, "hash", verusHash);
}

NODE_MODULE(verushash, Init)
