#include <node.h>
#include <node_events.h>
#include <assert.h>
#include <string.h>

using namespace v8;
using namespace node;

void hex_encode(unsigned char *md_value, int md_len, char** md_hexdigest, int* md_hex_len) {
  *md_hex_len = (2*(md_len));
  *md_hexdigest = (char *) malloc(*md_hex_len + 1);
  for(int i = 0; i < md_len; i++) {
    sprintf((char *)(*md_hexdigest + (i*2)), "%02x",  md_value[i]);
  }
}

#define hex2i(c) ((c) <= '9' ? ((c) - '0') : (c) <= 'Z' ? ((c) - 'A' + 10) : ((c) - 'a' + 10))
void hex_decode(unsigned char *input, int length, char** buf64, int* buf64_len) {
  *buf64_len = (length/2);
  *buf64 = (char*) malloc(length/2 + 1);
  char *b = *buf64;
  for(int i = 0; i < length-1; i+=2) {
    b[i/2]  = (hex2i(input[i])<<4) | (hex2i(input[i+1]));
  }
}

class Hash : public ObjectWrap {
 public:
  static void
  Initialize (v8::Handle<v8::Object> target)
  {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    t->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(t, "init", HashInit);
    NODE_SET_PROTOTYPE_METHOD(t, "update", HashUpdate);
    NODE_SET_PROTOTYPE_METHOD(t, "digest", HashDigest);

    target->Set(String::NewSymbol("Hash"), t->GetFunction());
  }

  bool HashInit (const char* hashType)
  {
    initialised = true;
    return true;
    
  }

  int HashUpdate(char* data, int len) {
    if (!initialised)
      return 0;
    // put Hash here
    return 1;
  }

  int HashDigest(unsigned char** md_value, unsigned int *md_len) {
    if (!initialised)
      return 0;
    // finalize and return value here
    return 1;
  }


 protected:

  static Handle<Value>
  New (const Arguments& args)
  {
    HandleScope scope;

    Hash *hash = new Hash();
    hash->Wrap(args.This());
    return args.This();
  }

  static Handle<Value>
  HashInit(const Arguments& args) {
    Hash *hash = ObjectWrap::Unwrap<Hash>(args.This());

    HandleScope scope;

    if (args.Length() == 0 || !args[0]->IsString()) {
      return ThrowException(String::New("Must give hashtype string as argument"));
    }

    String::Utf8Value hashType(args[0]->ToString());

    bool r = hash->HashInit(*hashType);

    return args.This();
  }

  static Handle<Value>
  HashUpdate(const Arguments& args) {
    Hash *hash = ObjectWrap::Unwrap<Hash>(args.This());

    HandleScope scope;

    enum encoding enc = ParseEncoding(args[1]);
    ssize_t len = DecodeBytes(args[0], enc);

    if (len < 0) {
      Local<Value> exception = Exception::TypeError(String::New("Bad argument"));
      return ThrowException(exception);
    }

    char* buf = new char[len];
    ssize_t written = DecodeWrite(buf, len, args[0], enc);
    assert(written == len);

    int r = hash->HashUpdate(buf, len);

    return args.This();
  }

  static Handle<Value>
  HashDigest(const Arguments& args) {
    Hash *hash = ObjectWrap::Unwrap<Hash>(args.This());

    HandleScope scope;

    unsigned char* md_value;
    unsigned int md_len;
    char* md_hexdigest;
    int md_hex_len;
    Local<Value> outString ;

    int r = hash->HashDigest(&md_value, &md_len);

    if (md_len == 0 || r == 0) {
      return scope.Close(String::New(""));
    }

    if (args.Length() == 0 || !args[0]->IsString()) {
      // Binary
      outString = Encode(md_value, md_len, BINARY);
    } else {
      String::Utf8Value encoding(args[0]->ToString());
      if (strcasecmp(*encoding, "hex") == 0) {
        // Hex encoding
        hex_encode(md_value, md_len, &md_hexdigest, &md_hex_len);
        outString = Encode(md_hexdigest, md_hex_len, BINARY);
        free(md_hexdigest);
      } else if (strcasecmp(*encoding, "binary") == 0) {
        outString = Encode(md_value, md_len, BINARY);
      } else {
	fprintf(stderr, "verushash-node : Hash .digest encoding "
		"can be binary or hex\n");
      }
    }
    free(md_value);
    return scope.Close(outString);

  }

  Hash () : ObjectWrap (), vh()
  {
    initialised = false;
  }

  ~Hash ()
  {
  }

 private:
  CVerusHash vh;
  bool initialised;
};

extern "C" void
init (Handle<Object> target) 
{
  HandleScope scope;
  Hash::Initialize(target);
}
