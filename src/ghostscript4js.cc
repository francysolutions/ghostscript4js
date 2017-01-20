/*******************************************************************************
 * Copyright (c) 2016 Nicola Del Gobbo
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the license at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS
 * OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY
 * IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
 * MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 *
 * Contributors - initial API implementation:
 * Nicola Del Gobbo <nicoladelgobbo@gmail.com>
 ******************************************************************************/

#include "ghostscript4js.h"

static int GSDLLCALL
gsdll_stdin(void *instance, char *buf, int len)
{
    int ch;
    int count = 0;
    while (count < len) {
	ch = fgetc(stdin);
	if (ch == EOF)
	    return 0;
	*buf++ = ch;
	count++;
	if (ch == '\n')
	    break;
    }
    return count;
}

static int GSDLLCALL
gsdll_stdout(void *instance, const char *str, int len)
{
    fwrite(str, 1, len, stdout);
    fflush(stdout);
    return len;
}

static int GSDLLCALL
gsdll_stderr(void *instance, const char *str, int len)
{
    fwrite(str, 1, len, stderr);
    fflush(stderr);
    return len;
}

void *minst;
const char start_string[] = "systemdict /start get exec\n";


void Version(const Nan::FunctionCallbackInfo<Value> &info)
{
    Nan::HandleScope();
    gsapi_revision_t r;
   gsapi_revision(&r, sizeof(r));
    info.GetReturnValue().Set(Nan::New<String>(r.product).ToLocalChecked());
}

void Execute(const Nan::FunctionCallbackInfo<Value> &info)
{
    Nan::HandleScope();
    

    info.GetReturnValue().Set(Nan::New<String>("Execute Ghostscript command").ToLocalChecked());
}

void ExecuteSync(const Nan::FunctionCallbackInfo<Value> &info)
{
    Nan::HandleScope();
    int code, code1;
    int exit_code;
    code = gsapi_new_instance(&minst, NULL);
    if (code < 0)
	    info.GetReturnValue().Set(Nan::New<String>("Error code < 0").ToLocalChecked());
    gsapi_set_stdio(minst, gsdll_stdin, gsdll_stdout, gsdll_stderr);
    code = gsapi_set_arg_encoding(minst, GS_ARG_ENCODING_UTF8);    

    info.GetReturnValue().Set(Nan::New<String>("Sync exection of Ghostscript command").ToLocalChecked());
}


//////////////////////////// INIT & CONFIG MODULE //////////////////////////////

void Init(Local<Object> exports)
{

    exports->Set(Nan::New("version").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(Version)->GetFunction());

    exports->Set(Nan::New("execute").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(Version)->GetFunction());

    exports->Set(Nan::New("executeSync").ToLocalChecked(),
                 Nan::New<FunctionTemplate>(ExecuteSync)->GetFunction());              
}

NODE_MODULE(ghostscript4js, Init)

////////////////////////////////////////////////////////////////////////////////
