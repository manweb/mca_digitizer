void PreampCalibration()
{
    gStyle->SetOptStat(0);
    
    double q = 1.60217662e-19;
    double c1 = 2e-12;
    double c2 = 1e-12;
    
    //double x1[4] = {712.8,1355.2,1887.7,2640.5};
    //double y1[4] = {1e-3*c1/q,2e-3*c1/q,3e-3*c1/q,4e-3*c1/q};
    
    //double x1[5] = {7.68029e+03,5.66704e+03,4.21225e+03,2.95180e+03,874.4};
    //double y1[5] = {1e-3*c1/q,0.7077e-3*c1/q,0.50123e-3*c1/q,0.31636e-3*c1/q,0};
    
    //double x1_err[5] = {1,1,1,1,1};
    //double y1_err[5] = {0.01e-3*c1/q,0.01e-3*c1/q,0.01e-3*c1/q,0.01e-3*c1/q,100};
    
    double att[5] = {0.31622776601,0.22387211385,0.15848931924,0.11220184543,0.07943282347};
    
    double x1[5] = {4.73820e+03,3.33227e+03,2.32865e+03,1.62825e+03,1.12274e+03};
    double y1[5] = {att[0]*c1/q,att[1]*c1/q,att[2]*c1/q,att[3]*c1/q,att[4]*c1/q};
    
    double x1_err[5] = {1,1,1,1,1};
    double y1_err[5] = {0.01*att[0]*c1/q,0.01*att[1]*c1/q,0.01*att[2]*c1/q,0.01*att[3]*c1/q,0.01*att[4]*c1/q};
    
    //double x1_err[4] = {26.26,26.22,26.44,26.47};
    //double y1_err[4] = {0.2e-3*c1/q,0.2e-3*c1/q,0.2e-3*c1/q,0.2e-3*c1/q};
    
    //double x2[4] = {347,643,936,1320};
    //double y2[4] = {1e-3*c2/q,2e-3*c2/q,3e-3*c2/q,4e-3*c2/q};
    
    //double x2_err[4] = {34,49,49,50};
    //double y2_err[4] = {0.1e-3*c2/q,0.1e-3*c2/q,0.1e-3*c2/1,0.1e-3*c2/q};
    
    //double x2[8] = {2.28807e+03,1.48678e+03,1.07409e+03,7.79458e+02,5.74264e+02,4.28424e+02,3.05739e+02,96.16};
    //double y2[8] = {3.162e-3*c1/q,1.995e-3*c1/q,1.413e-3*c1/q,1e-3*c1/q,0.7077e-3*c1/q,0.50123e-3*c1/q,0.31636e-3*c1/q,0};
    
    //double x2_err[8] = {1,1,1,1,1,1,1,1};
    //double y2_err[8] = {0.01e-3*c1/q,0.01e-3*c1/q,0.01e-3*c1/q,0.01e-3*c1/q,0.01e-3*c1/q,0.01e-3*c1/q,0.01e-3*c1/q,100};
    
    double x2[5] = {6.53312e+03,4.56928e+03,3.19422e+03,2.23389e+03,1.54368e+03};
    double y2[5] = {att[0]*c1/q,att[1]*c1/q,att[2]*c1/q,att[3]*c1/q,att[4]*c1/q};
    
    double x2_err[5] = {1,1,1,1,1};
    double y2_err[5] = {0.01*att[0]*c1/q,0.01*att[1]*c1/q,0.01*att[2]*c1/q,0.01*att[3]*c1/q,0.01*att[4]*c1/q};
    
    TGraphErrors *gr1 = new TGraphErrors(5,x1,y1,x1_err,y1_err);
    
    TGraphErrors *gr2 = new TGraphErrors(5,x2,y2,x2_err,y2_err);
    
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerSize(0.5);
    
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerSize(0.5);
    gr2->SetMarkerColor(kBlack);
    gr2->SetLineColor(kBlack);
    
    TF1 *fit1 = new TF1("fit1","[0]*x+[1]",0,8400);
    
    TF1 *fit2 = new TF1("fit2","[0]*x+[1]",0,8400);
    
    fit1->SetParameters(800,0);
    
    fit2->SetParameters(800,0);
    
    //fit1->FixParameter(1,0);
    
    //fit2->FixParameter(1,0);
    
    fit1->SetLineWidth(1);
    fit1->SetLineColor(kBlack);
    fit1->SetLineStyle(2);
    
    fit2->SetLineWidth(1);
    fit2->SetLineColor(kBlack);
    fit2->SetLineStyle(2);
    
    gr1->Fit(fit1,"r","goff");
    
    gr2->Fit(fit2,"r","goff");
    
    TH2D *h_empty = new TH2D("h_empty","h_empty",100,300,3500,100,1e3,6e4);
    
    TCanvas *c3 = new TCanvas();
    
    //h_empty->Draw();
    //gr1->Draw("AZP");
    gr2->Draw("AZPsame");
    //fit1->Draw("same");
    fit2->Draw("same");
}
