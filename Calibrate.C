void Calibrate()
{
    gStyle->SetOptStat(0);
    //gStyle->SetOptTitle(0);
    gStyle->SetOptFit(1);
    
    string baseFilename = "050317/NoGain/calib/FS10V/LXe_Bi207_calib_1V_[wildcard]_G50_1000VNG_050317_001.root";
    
    string outName1 = "050317/NoGain/plots/calib/LXe_Bi207_calib_1V_G50_10VFS_NoGain_050317_001.root";
    string outName2 = outName1;
    
    outName2.replace(outName2.find(".root"),5,"_spectra.root");
    
    string title = "no gain, shaper2, 10V FS";
    
    bool useHistos = true;
    
    const int nFiles = 7;
    
    double att[nFiles] = {53, 50, 46, 43, 36, 26, 23};
    double calibParsInit1[nFiles] = {140,190,300,430,970,3080,4370};
    double calibParsInit2[nFiles] = {80,120,190,260,580,1870,2650};
    
    double C = 2e-12; // 2pF
    double q = 1.60217662e-19;
    
    TH1F *h[nFiles];
    TF1 *fit[nFiles];
    
    TGraphErrors *gr = new TGraphErrors(nFiles);
    
    gr->SetTitle(title.c_str());
    
    for (int i = 0; i < nFiles; i++) {
        string filename = baseFilename;
        
        filename.replace(filename.find("[wildcard]"),10,Form("%.0fdB",att[i]));
        
        cout << "Reading file " << filename.c_str() << endl;
        
        TFile *f = new TFile(filename.c_str(),"READ");
        
        TTree *t = (TTree*)f->Get("dataTree");
        
        if (!useHistos) {
            h[i] = new TH1F(Form("h_%i",i),Form("h_%i",i),4000,0,8000);
            
            t->Draw(Form("rawData>>h_%i",i),"","goff");
        }
        else {
            h[i] = (TH1F*)f->Get("hRawData")->Clone();
            h[i]->SetName(Form("h_%i",i));
            h[i]->SetTitle(Form("h_%i",i));
        }
        
        fit[i] = new TF1(Form("fit_%i",i),"[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Gaus(x,[4],[5])",20,8000);
        
        double scale = 1;
        if (i > 0) {scale = TMath::Power(10,-(att[i]-att[0])/20);}
        
        fit[i]->SetParameters(1400,calibParsInit1[i],5,1400,calibParsInit2[i],5);
        
        fit[i]->SetParLimits(2,1,200);
        fit[i]->SetParLimits(5,1,200);
        
        h[i]->Fit(fit[i],"rn","goff");
        
        h[i]->SetLineColor(kBlack);
        h[i]->SetMarkerStyle(20);
        h[i]->SetMarkerSize(0.5);
        
        h[i]->GetXaxis()->SetRangeUser(fit[i]->GetParameter(4)-10*fit[i]->GetParameter(5),fit[i]->GetParameter(1)+10*fit[i]->GetParameter(2));
        
        fit[i]->SetLineColor(kBlue);
        fit[i]->SetLineWidth(2);
        
        double voltage = TMath::Power(10,-att[i]/20);
        
        double N = C*voltage/q;
        
        gr->SetPoint(i,fit[i]->GetParameter(1),N);
        gr->SetPointError(i,fit[i]->GetParError(1),N*0.01);
    }
    
    TCanvas *c2 = new TCanvas();
    
    c2->Divide(2,ceil(double(nFiles)/2));
    
    for (int i = 1; i < nFiles+1; i++) {
        c2->cd(i);
        
        h[i-1]->Draw("EZP");
        fit[i-1]->Draw("same");
    }
    
    TF1 *fit2 = new TF1("fit2","[0]+[1]*x",0,8000);
    
    fit2->FixParameter(0,0);
    
    fit2->SetLineColor(kBlack);
    fit2->SetLineWidth(2);
    fit2->SetLineStyle(2);
    
    gr->Fit(fit2,"r","goff");
    
    gr->SetLineColor(kBlack);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(0.5);
    
    gr->GetXaxis()->SetTitle("ADC channel");
    gr->GetYaxis()->SetTitle("number of electrons");
    
    gr->GetXaxis()->CenterTitle();
    gr->GetYaxis()->CenterTitle();
    
    gr->GetYaxis()->SetTitleOffset(1.4);
    
    /*fit2->SetLineColor(kBlack);
    fit2->SetLineWidth(1);
    fit2->SetLineStyle(2);*/
    
    TCanvas *c3 = new TCanvas();
    
    gr->Draw("AZP");
    fit2->Draw("same");
    
    c2->SaveAs(outName2.c_str());
    
    c3->SaveAs(outName1.c_str());
}
