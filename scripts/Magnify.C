void Magnify(const char* filename="../tmp/test.root");
void Magnify(const char* filename)
{
    // Data *data = new Data(filename);

    GuiController *gc = new GuiController(
        gClient->GetRoot(),
        1600,
        900,
	    filename
    );

}
