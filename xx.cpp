#include "pch.h"
#include "MyForm.h"
#include "Tournament.h"

#include <cliext/vector>
#include <cliext/map>
#include <cliext/utility>
#include <fstream>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace cliext;
using namespace System::IO;
using namespace System::Windows::Forms;


int main() {

    Application::SetCompatibleTextRenderingDefault(false);
    Application::EnableVisualStyles();
    ITProjektaktuell::MyForm frm;
    Application::Run(% frm);

    return 0;
}
