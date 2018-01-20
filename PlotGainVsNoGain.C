void PlotGainVsNoGain()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
    const int nFiles = 1;
    
    //string path = "032117/G50/";
    string path = "050317/";
    
    //string input1[nFiles] = {   "LXe_Bi207_Shaper1_G50_10VFS_1350VG1_650VG2_032117_002.root",
    //                            "LXe_Bi207_Shaper1_G50_10VFS_1350VG1_650VG2_032117_003.root"};
    //string input2[nFiles] = {   "LXe_Bi207_Shaper2_G50_10VFS_1000VNG_032117_002.root",
    //                            "LXe_Bi207_Shaper2_G50_10VFS_1000VNG_032117_003.root"};
    
    string input1[nFiles] = {   "Gain/LXe_Bi207_Shaper1_G50_10VFS_550VG1_1450VG2_050317_004.root"};
    string input2[nFiles] = {   "NoGain/LXe_Bi207_Shaper2_G50_10VFS_1000VNG_050317_004.root"};
    
    //string input1 = "032117/G50/LXe_Bi207_Shaper1_G50_10VFS_1350VG1_650VG2_032117_003.root"; // Gain file
    //string input2 = "032117/G50/LXe_Bi207_Shaper2_G50_10VFS_1000VNG_032117_003.root"; // Non-gain file
    
    string NEST_table = "NEST_table_1kVcm.txt";
    string MC = "Bi207_MCHist_10mm_2.root";
    
    double calib_p0_gain = 8176;
    double calib_p1_gain = 223.8;
    double calib_p0_nonGain = 0;
    double calib_p1_nonGain = 202.3;
    
    double xmin = 0;
    double xmax = 200000;
    double ymin = 1;
    double ymax = 1e7;
    
    TFile *f1;
    TFile *f2;
    TH1F *h1_tmp;
    TH1F *h2_tmp;
    
    // Open the first file
    f1 = new TFile((path+input1[0]).c_str(),"READ");
    f2 = new TFile((path+input2[0]).c_str(),"READ");
    
    TH1F *h1 = (TH1F*)f1->Get("hDataCalib");
    TH1F *h2 = (TH1F*)f2->Get("hDataCalib");
    
    //f1->Close();
    //f2->Close();
    
    // Open all other files
    for (int i = 1; i < nFiles; i++) {
        f1 = new TFile((path+input1[i]).c_str(),"READ");
        f2 = new TFile((path+input2[i]).c_str(),"READ");
        
        h1_tmp = (TH1F*)f1->Get("hDataCalib");
        h2_tmp = (TH1F*)f2->Get("hDataCalib");
        
        h1->Add(h1_tmp);
        h2->Add(h2_tmp);
        
        //f1->Close();
        //f2->Close();
    }
    
    TTree *t = new TTree();
    
    t->ReadFile(NEST_table.c_str());
    
    t->Draw("energy:ionization*energy","","goff");
    
    TGraph *gr1 = new TGraph(t->GetSelectedRows(),t->GetV2(),t->GetV1());
    TGraph *gr2 = new TGraph(t->GetSelectedRows(),t->GetV1(),t->GetV2());
    
    TFile *f3 = new TFile(MC.c_str(),"READ");
    
    TH1F *h3 = (TH1F*)f3->Get("h");
    
    double nBins = h3->GetNbinsX();
    double hXmin = h3->GetXaxis()->GetXmin();
    double hXmax = h3->GetXaxis()->GetXmax();
    double binSize = h3->GetXaxis()->GetBinWidth(0);
    
    TH1F *h4 = new TH1F("h4","h4",nBins,0,gr2->Eval(hXmax*1000));
    
    for (int i = 0; i < nBins; i++) {
        double binContent = h3->GetBinContent(i);
        double energy = hXmin + i*binSize + binSize/2;
        double electrons = gr2->Eval(energy*1000);
        
        h4->SetBinContent(h4->FindBin(electrons),binContent);
    }
    
    double MCscale = 1.0;
    
    h4->Scale(MCscale);
    
    h4->SetLineColor(kGreen+1);
    
    h4->Rebin(5);
    
    double scale = h1->Integral(h1->FindBin(25000),h1->FindBin(35000)) / h2->Integral(h2->FindBin(25000),h2->FindBin(35000));
    
    h2->Scale(scale);
    
    h1->SetLineColor(kBlack);
    h2->SetLineColor(kRed+1);
    
    TGaxis *axis = new TGaxis(xmin,ymax,xmax,ymax,0,gr1->Eval(xmax),510,"-",0);
    
    axis->SetTitle("energy (keV)");
    axis->CenterTitle();
    axis->SetLabelFont(42);
    axis->SetTitleFont(42);
    axis->SetLabelSize(0.035);
    axis->SetTitleSize(0.035);
    axis->SetTitleOffset(1.2);
    
    TH2D *h_empty = new TH2D("h_empty","h_empty",100,xmin,xmax,100,ymin,ymax);
    
    h_empty->GetXaxis()->SetTitle("number of electrons");
    h_empty->GetYaxis()->SetTitle("counts");
    
    h_empty->GetXaxis()->CenterTitle();
    h_empty->GetYaxis()->CenterTitle();
    
    TLegend *l = new TLegend(0.6,0.65,0.85,0.85);
    
    l->AddEntry(h1,"Gain","l");
    l->AddEntry(h2,"Non-Gain","l");
    l->AddEntry(h4,"MC","l");
    
    l->SetFillStyle(0);
    
    TCanvas *c1 = new TCanvas();
    
    c1->SetLogy();
    c1->SetGridy();
    c1->SetGridx();
    
    h_empty->Draw();
    h1->Draw("same");
    h2->Draw("same");
    h4->Draw("same");
    axis->Draw("same");
    l->Draw("same");
}
