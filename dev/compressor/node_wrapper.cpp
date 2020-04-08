#include <nan.h>
#include <uv.h>
#include <string>

#include "huffman.h"

struct CompressorData 
{
    uv_work_t request;
    v8::Persistent<v8::Function> callback;
    int result;
    std::string filename;
};

static void CompressAsyncComplete(uv_work_t *req, int status)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);

    CompressorData* work = static_cast<CompressorData*>(req->data);

    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { v8::Number::New(isolate, work->result) };

    v8::Local<v8::Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext(), v8::Null(isolate), argc, argv);

    work->callback.Reset();
    delete work;
}

static void DecompressAsyncComplete(uv_work_t *req, int status)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);

    CompressorData* work = static_cast<CompressorData*>(req->data);

    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { v8::Number::New(isolate, work->result) };

    v8::Local<v8::Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext(), v8::Null(isolate), argc, argv);

    work->callback.Reset();
    delete work;
}

static void CompressAsync(uv_work_t *req) 
{
    CompressorData *data = static_cast<CompressorData*>(req->data);
    try
    {
        huffman huf(data->filename.c_str());
        data->result = huf.compress();
    }
    catch(...)
    {
        data->result = -4;
    }
}

static void DecompressAsync(uv_work_t *req) 
{
    CompressorData *data = static_cast<CompressorData*>(req->data);
    try
    {
        huffman huf(data->filename.c_str());
        data->result = huf.decompress();
    }
    catch(...)
    {
        data->result = -4;
    }
}

void StartCompress(const Nan::FunctionCallbackInfo<v8::Value>&info)
{
    v8::Isolate* isolate = info.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    CompressorData* work = new CompressorData();
    work->request.data = work;

    if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsFunction())
    {
        info.GetReturnValue().Set(Nan::New(-3));
        return;
    }

    work->filename = std::string(*v8::String::Utf8Value(isolate, info[0]));
    v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(info[1]);
    work->callback.Reset(isolate, callback);

    uv_queue_work(uv_default_loop(), &work->request, CompressAsync, CompressAsyncComplete);
    info.GetReturnValue().Set(Undefined(isolate));  
}

void StartDecompress(const Nan::FunctionCallbackInfo<v8::Value>&info)
{
    v8::Isolate* isolate = info.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    CompressorData* work = new CompressorData();
    work->request.data = work;

    if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsFunction())
    {
        info.GetReturnValue().Set(Nan::New(-3));
        return;
    }

    work->filename = std::string(*v8::String::Utf8Value(isolate, info[0]));
    v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(info[1]);
    work->callback.Reset(isolate, callback);

    uv_queue_work(uv_default_loop(), &work->request, DecompressAsync, DecompressAsyncComplete);
    info.GetReturnValue().Set(Undefined(isolate));  
}

void Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::Context> context = exports->CreationContext();
  exports->Set(context,
               Nan::New("compress").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(StartCompress)
                   ->GetFunction(context)
                   .ToLocalChecked());
  exports->Set(context,
               Nan::New("decompress").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(StartDecompress)
                   ->GetFunction(context)
                   .ToLocalChecked());
}

NODE_MODULE(addon, Init)