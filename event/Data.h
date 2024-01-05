#ifndef DATA_H
#define DATA_H

class TFile;
class TCanvas;


class Data {
public:
    Data();
    Data(const char* filename);

    virtual ~Data();


    TCanvas *c1;
    TFile *rootFile;

    void Draw1(int n=5000);


private:
    void LoadData(const char* filename);

};

#endif
