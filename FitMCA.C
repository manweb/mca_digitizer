void FitMCA()
{
    string inputFile;
    
    cout << "Enter input file:" << endl;
    getline(cin,inputFile);
    
    TFile *f = new TFile(inputFile.c_str(),"READ");
    
    TTree *t = (TTree*)f->Get("dataTree");
    
    int nChannels;
    t->SetBranchAddress("MCAC",&nChannels);
    
    t->GetEntry(0);
    
    TH1I *h = new TH1I("h","h",nChannels,0,nChannels);
    
    h->SetMarkerStyle(20);
    h->SetMarkerSize(0.5);
    h->SetMarkerColor(kBlack);
    
    t->Draw("rawData >> h","","goff");
    
    TF1 *fit = new TF1("fit","[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Gaus(x,[4],[5])",200,nChannels);
    //TF1 *fit = new TF1("fit","[0]*TMath::Gaus(x,[1],[2])",2800,nChannels);
    
    fit->SetParameters(400,920,20,400,1540,20);
    fit->SetLineColor(kBlue);
    
    h->Fit(fit,"RN");
    
    TCanvas *c1 = new TCanvas();
    h->Draw("EZP");
    fit->Draw("same");
}