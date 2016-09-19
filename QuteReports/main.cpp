#include "src/quteapplication.h"

int main(int argc, char *argv[])
{
    QuteApplication::registerCustomQmlTypes();
    QuteApplication app{argc, argv};
    app.bootstrap();
    app.run();
}
