#include <fstream>
#include <string>
#include <iostream>

void RootifyMCA()
{
    string inputFile = "";
    string outputFile = "";
    
    string calib_p0_string = "";
    string calib_p1_string = "";
    
    string saveHistos = "";
    
    bool outputHistos = false;
    
    cout << "Enter input file:" << endl;
    getline(cin, inputFile);
    
    cout << "Calibration paramter 0 (constant):" << endl;
    getline(cin, calib_p0_string);
    
    cout << "Calibration parameter 1 (slope):" << endl;
    getline(cin, calib_p1_string);
    
    cout << "Save histos? (0 = false, 1 = true)" << endl;
    getline(cin, saveHistos);
    
    if (atof(saveHistos.c_str()) == 1) {outputHistos = true;}
    
    double calib_p0 = atof(calib_p0_string.c_str());
    double calib_p1 = atof(calib_p1_string.c_str());
    
    cout << "Using calibration N = ADC*" << calib_p1 << "+" << calib_p0 << endl;
    
    TSystemDirectory dir(inputFile.c_str(), inputFile.c_str());
    TList *files = dir.GetListOfFiles();
    
    if (files) {
        TSystemFile *file;
        TString fname;
        TIter next(files);
        while ((file = (TSystemFile*)next())) {
            fname = file->GetName();
            if (!file->IsDirectory() && fname.EndsWith(".mca")) {
                string filename = inputFile+fname.Data();
                ConvertFile(filename,calib_p0,calib_p1,outputHistos);
            }
        }
    }
    else {ConvertFile(inputFile,calib_p0,calib_p1,outputHistos);}
    
    exit();
}

void ConvertFile(string inputFile, double calib_p0, double calib_p1, bool histos)
{
    string outputFile = inputFile;
    outputFile.replace(outputFile.find(".mca"),4,".root");
    
    cout << "Converting file " << inputFile << " to " << outputFile << endl;
    
    std::ifstream infile(inputFile.c_str());
    
    if (!infile) {cout << "File " << inputFile << " does not exist." << endl;}
    
    int mcac;
    int gaia;
    double thsl;
    int gate;
    int prcl;
    int prch;
    int mcsl;
    int mcsh;
    int rawData;
    double data;
    double dataCalib;
    
    TTree *t = new TTree("dataTree","dataTree");
    
    t->Branch("MCAC",&mcac,"MCAC/I");
    t->Branch("GAIA",&gaia,"GAIA/I");
    t->Branch("THSL",&thsl,"THSL/D");
    t->Branch("GATE",&gate,"GATE/I");
    t->Branch("PRCL",&prcl,"PRCL/I");
    t->Branch("PRCH",&prch,"PRCH/I");
    t->Branch("MCSL",&mcsl,"MCSL/I");
    t->Branch("MCSH",&mcsh,"MCSH/I");
    
    if (!histos) {
        t->Branch("rawData",&rawData,"rawData/I");
        t->Branch("data",&data,"data/D");
        t->Branch("dataCalib",&dataCalib,"dataCalib/D");
    }
    
    string line;
    int configurationTag = 0;
    while (infile >> line) {
        if (line.find("<<DP5") != string::npos && configurationTag != 1) {configurationTag = 1; continue;}
        if (line.find("<<DP5") != string::npos && configurationTag == 2) {configurationTag = 3;}
        
        if (configurationTag == 0) {continue;}
        if (configurationTag == 1) {configurationTag = 2; continue;}
        if (configurationTag == 3) {continue;}
        if (line.find("GATE?") != string::npos) {continue;}
        
        if (line.find("MCAC") != string::npos) {
            line.replace(0,5,"");
            line.replace(line.find(";"),1,"");
            
            mcac = atoi(line.c_str());
            cout << "MCAC = " << mcac << endl;
        }
        if (line.find("GAIA") != string::npos) {
            line.replace(0,5,"");
            line.replace(line.find(";"),1,"");
            
            gaia = atoi(line.c_str());
            cout << "GAIA = " << gaia << endl;
        }
        if (line.find("THSL") != string::npos) {
            line.replace(0,5,"");
            line.replace(line.find(";"),1,"");
            
            thsl = atof(line.c_str());
            cout << "THSL = " << thsl << endl;
        }
        if (line.find("GATE") != string::npos) {
            line.replace(0,5,"");
            line.replace(line.find(";"),1,"");
            
            if (line.find("OFF") != string::npos) {gate = 0;}
            if (line.find("HI") != string::npos) {gate = 1;}
            if (line.find("LO") != string::npos) {gate = 2;}
            
            cout << "GATE = " << gate << endl;
        }
        if (line.find("PRCL") != string::npos) {
            line.replace(0,5,"");
            line.replace(line.find(";"),1,"");
            
            prcl = atoi(line.c_str());
            cout << "PRCL = " << prcl << endl;
        }
        if (line.find("PRCH") != string::npos) {
            line.replace(0,5,"");
            line.replace(line.find(";"),1,"");
            
            prch = atoi(line.c_str());
            cout << "PRCH = " << prch << endl;
        }
        if (line.find("MCSL") != string::npos) {
            line.replace(0,5,"");
            line.replace(line.find(";"),1,"");
            
            mcsl = atoi(line.c_str());
            cout << "MCSL = " << mcsl << endl;
        }
        if (line.find("MCSH") != string::npos) {
            line.replace(0,5,"");
            line.replace(line.find(";"),1,"");
            
            mcsh = atoi(line.c_str());
            cout << "MCSH = " << mcsh << endl;
        }
    }
    
    infile.clear();
    infile.seekg(0,ios::beg);
    
    if (histos) {
        TH1D *h_rawData = new TH1D("hRawData","hRawData",mcac,1,mcac);
        TH1D *h_data = new TH1D("hData","hData",mcac,double(gaia)/double(mcac),double(gaia));
        TH1D *h_dataCalib = new TH1D("hDataCalib","hDataCalib",mcac,floor(calib_p1+calib_p0),ceil(mcac*calib_p1+calib_p0));
    }
    
    bool dataTag = false;
    int bin = mcsl;
    while (infile >> line) {
        if (line.find("<<DATA>>") != string::npos && !dataTag) {dataTag = true; continue;}
        if (line.find("<<END>>") != string::npos) {dataTag = false;}
        
        if (!dataTag) {continue;}
        int binValue = atoi(line.c_str());
        
        if (histos) {
            h_rawData->SetBinContent(bin,binValue);
            h_data->SetBinContent(h_data->FindBin(double(gaia)/double(mcac)*bin),binValue);
            h_dataCalib->SetBinContent(h_dataCalib->FindBin(bin*calib_p1+calib_p0),binValue);
        }
        else {
            for (int i = 0; i < binValue; i++) {
                rawData = bin;
                data = double(gaia)/double(mcac)*bin;
                dataCalib = rawData*calib_p1 + calib_p0;
                
                t->Fill();
            }
        }
        
        bin++;
    }
    
    if (histos) {t->Fill();}
    
    TFile *f = new TFile(outputFile.c_str(),"RECREATE");
    
    if (histos) {
        h_rawData->Write();
        h_data->Write();
        h_dataCalib->Write();
    }
    
    t->Write();
    
    f->Close();
}
