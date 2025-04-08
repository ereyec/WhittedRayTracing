#include <iostream>
#include <string>	

#include "common.h"
#include "context.h"
#include "application.h"

int main(int argc, char* argv[]){
	Application app;
	
	if(app.init(context) != -1){
		app.run(context);
	}
	app.cleanup(context);

	return 0;
}