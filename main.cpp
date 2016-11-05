#include "lispp/SchemeInterpreter.h"

int main() { 
	
	SchemeInterpreter inter;
	inter.usePrompt = true;
	inter.debugInfo = false;
	inter.Run();
	
    return 0;
}
