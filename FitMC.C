void FitMC()
{
    TFile *f = new TFile("Bi207_MCHist_10mm.root");
    
    TH1F *hMC = f->Get("h");
    
    hMC->Rebin(50);
    hMC->GetXaxis()->SetRangeUser(0,3);
    
    RooRealVar energy("energy","energy",0,3);
    
    RooRealVar GausMean("GausMean","mean",0);
    RooRealVar GausSigma("GausSigma","sigma",0.1,0.01,1);
    RooGaussian Gaus("Gaus","Gaussian PDF",energy,GausMean,GausSigma);
    
    RooDataHist MCdata("MCdata","MC",energy,hMC);
    RooHistPdf MCPDF("MCPDF","MC pdf",energy,MCdata);
    
    RooFFTConvPdf MCsmeared("MCsmeared","MC smeared",energy,MCPDF,Gaus);
    
    RooPlot *frame = energy.frame();
    MCsmeared.plotOn(frame);
    
    TCanvas *c1 = new TCanvas();
    
    frame->Draw();
}
