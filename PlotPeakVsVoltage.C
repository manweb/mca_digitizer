void PlotPeakVsVoltage()
{
    // Plots MCA spectra of light signal taken at different bias voltages and performs
    // double Gaussian fit
    
    // Input file name
    //string inputFile = "072415/flash_lamp_*V_Cube_1V.root";
    //string inputFile = "072415/flash_lamp_1200V_Cube_*V.root";
    string inputFile = "073115/flash_lamp_gain_*V_*V_Cube.root";
    //string inputFile = "073115/flash_lamp_gain_1200V_*V_Cube.root";
    
    const int n = 5;
    
    // Bias voltages
    //double voltages[n] = {1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200};
    //double voltages[n] = {2, 3, 4};
    double voltages[n] = {1000, 1100, 1200, 1300, 1400};
    //double voltages[n] = {1200, 1100, 1000, 900, 800, 700, 600};
    
    // Histograms
    TH1I *h[n];
    
    // Fit functions
    TF1 *fit[n];
    string fitFunction = "[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Gaus(x,[4],[5])";
    
    // Fit results
    double *Peak1Mean = new double[n];
    double *Peak1Mean_err = new double[n];
    double *Peak1Sigma = new double[n];
    double *Peak1Sigma_err = new double[n];
    double *Peak2Mean = new double[n];
    double *Peak2Mean_err = new double[n];
    double *Peak2Sigma = new double[n];
    double *Peak2Sigma_err = new double[n];
    double *Peak1Res = new double[n];
    double *Peak1Res_err = new double[n];
    double *Peak2Res = new double[n];
    double *Peak2Res_err = new double[n];
    
    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    c1->Divide(5,3);
    
    // Loop over all files
    for (int i = 0; i < n; i++) {
        string file = inputFile;
        file.replace(file.find("*"),1,Form("%.0f",voltages[i]));
        file.replace(file.find("*"),1,Form("%.0f",voltages[i]));
        
        cout << "Opening file " << file << endl;
        
        TFile *f = new TFile(file.c_str(),"READ");
        
        TTree *t = (TTree*)f->Get("dataTree");
        
        int mcac;
        t->SetBranchAddress("MCAC",&mcac);
        
        t->GetEntry(0);
        
        const char *hist_name = Form("h_%.0f",voltages[i]);
        TH1I *h_temp = new TH1I(hist_name,hist_name,mcac,0,mcac);
        
        t->Draw(Form("rawData >> %s",hist_name),"","goff");
        
        h[i] = (TH1I*)h_temp->Clone();
        
        h[i]->SetMarkerStyle(20);
        h[i]->SetMarkerSize(0.5);
        h[i]->SetMarkerColor(kBlack);
        
        const char *fit_name = Form("fit_%.0f",voltages[i]);
        fit[i] = new TF1(fit_name,fitFunction.c_str(),0,mcac);
        
        fit[i]->SetParameters(5,700,150,25,1700+i*150,80);
        
        fit[i]->SetParLimits(1,500,1000);
        fit[i]->SetParLimits(2,10,200);
        fit[i]->SetParLimits(4,700,4000);
        fit[i]->SetParLimits(5,10,200);
        
        fit[i]->SetLineColor(kBlue);
        
        h[i]->Fit(fit_name,"RN");
        
        h[i]->SetTitle(Form("V_{bias} = %.0f, chi2/ndf = %.3f",voltages[i],fit[i]->GetChisquare()/fit[i]->GetNDF()));
        
        h[i]->GetXaxis()->SetRangeUser(500,3000);
        
        Peak1Mean[i] = fit[i]->GetParameter(1);
        Peak1Mean_err[i] = fit[i]->GetParError(1);
        Peak1Sigma[i] = fit[i]->GetParameter(2);
        Peak1Sigma_err[i] = fit[i]->GetParError(2);
        Peak2Mean[i] = fit[i]->GetParameter(4);
        Peak2Mean_err[i] = fit[i]->GetParError(4);
        Peak2Sigma[i] = fit[i]->GetParameter(5);
        Peak2Sigma_err[i] = fit[i]->GetParError(5);
        Peak1Res[i] = Peak1Sigma[i]/Peak1Mean[i];
        Peak1Res_err[i] = TMath::Sqrt(1/Peak1Mean[i]/Peak1Mean[i]*Peak1Sigma_err[i]*Peak1Sigma_err[i] + Peak1Sigma[i]*Peak1Sigma[i]/Peak1Mean[i]/Peak1Mean[i]/Peak1Mean[i]/Peak1Mean[i]*Peak1Mean_err[i]*Peak1Mean_err[i]);
        Peak2Res[i] = Peak2Sigma[i]/Peak2Mean[i];
        Peak2Res_err[i] = TMath::Sqrt(1/Peak2Mean[i]/Peak2Mean[i]*Peak2Sigma_err[i]*Peak2Sigma_err[i] + Peak2Sigma[i]*Peak2Sigma[i]/Peak2Mean[i]/Peak2Mean[i]/Peak2Mean[i]/Peak2Mean[i]*Peak2Mean_err[i]*Peak2Mean_err[i]);
        
        c1->cd(i+1);
        
        h[i]->Draw("EZP");
        fit[i]->Draw("same");
    }
    
    TGraphErrors *gr1 = new TGraphErrors(n,voltages,Peak1Mean,0,Peak1Mean_err);
    TGraphErrors *gr2 = new TGraphErrors(n,voltages,Peak1Sigma,0,Peak1Sigma_err);
    TGraphErrors *gr3 = new TGraphErrors(n,voltages,Peak2Mean,0,Peak2Mean_err);
    TGraphErrors *gr4 = new TGraphErrors(n,voltages,Peak2Sigma,0,Peak2Sigma_err);
    TGraphErrors *gr5 = new TGraphErrors(n,voltages,Peak1Res,0,Peak1Res_err);
    TGraphErrors *gr6 = new TGraphErrors(n,voltages,Peak2Res,0,Peak2Res_err);
    
    gr1->GetXaxis()->SetTitle("bias voltage [V]");
    gr1->GetYaxis()->SetTitle("pedestal mean");
    
    gr2->GetXaxis()->SetTitle("bias voltage [V]");
    gr2->GetYaxis()->SetTitle("pedestal sigma");
    
    gr3->GetXaxis()->SetTitle("bias voltage [V]");
    gr3->GetYaxis()->SetTitle("signal mean");
    
    gr4->GetXaxis()->SetTitle("bias voltage [V]");
    gr4->GetYaxis()->SetTitle("signal sigma");
    
    gr5->GetXaxis()->SetTitle("bias voltage [V]");
    gr5->GetYaxis()->SetTitle("pedestal res");
    
    gr6->GetXaxis()->SetTitle("bias voltage [V]");
    gr6->GetYaxis()->SetTitle("signal res");
    
    TCanvas *c2 = new TCanvas("c2","c2");
    gr1->Draw("AP");
    
    TCanvas *c3 = new TCanvas("c3","c3");
    gr2->Draw("AP");
    
    TCanvas *c4 = new TCanvas("c4","c4");
    gr3->Draw("AP");
    
    TCanvas *c5 = new TCanvas("c5","c5");
    gr4->Draw("AP");
    
    TCanvas *c6 = new TCanvas("c6","c6");
    gr5->Draw("AP");
    
    TCanvas *c7 = new TCanvas("c7","c7");
    gr6->Draw("AP");
}