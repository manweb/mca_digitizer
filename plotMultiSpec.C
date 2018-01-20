void plotMultiSpec()
{

  TTree *t = new TTree();
  t->ReadFile("Chamber_detached.txt");

  int channel, voltage1, voltage2, voltage3, voltage4, voltage5, voltage6, voltage7, voltage8, voltage9;

  t->SetBranchAddress("channel",&channel);
  t->SetBranchAddress("voltage1",&voltage1);
  t->SetBranchAddress("voltage2",&voltage2);
  t->SetBranchAddress("voltage3",&voltage3);
  t->SetBranchAddress("voltage4",&voltage4);
  t->SetBranchAddress("voltage5",&voltage5);
  t->SetBranchAddress("voltage6",&voltage6);
  t->SetBranchAddress("voltage7",&voltage7);
  t->SetBranchAddress("voltage8",&voltage8);
  t->SetBranchAddress("voltage9",&voltage9);

  const int n = 9;
  TH1I *h[n];
  for (int i = 0; i < n; i++) {
     h[i] = new TH1I(Form("h%i",i),Form("h%i",i),512,0,511);
  }

  for (int i = 0; i < t->GetEntries(); i++) {
     t->GetEntry(i);

     int voltage[] = {voltage1, voltage2, voltage3, voltage4, voltage5, voltage6, voltage7, voltage8, voltage9};

     for (int k = 0; k < n; k++) {
        h[k]->SetBinContent(i,voltage[k]);
     }
  }

  TLegend *l = new TLegend(0.8,0.8,0.9,0.9);

  h[(n-1)]->Draw();
  for (int k = 0; k < (n-1); k++) {
     double scale = h[(n-1)]->Integral()/h[k]->Integral();
     h[k]->Scale(scale);
     h[k]->SetLineColor(k+1);
     l->AddEntry(h[k],Form("%iV",100/5*k+600));

     h[k]->Rebin(4);

     //if (k == 1) {
        h[k]->Draw("same");
     //}
  }

  h[(n-1)]->Rebin(4);
  h[(n-1)]->SetLineColor(n);
  l->AddEntry(h[(n-1)],Form("%iV",100/5*(n-1)+600));

  l->Draw("Lsame");

}
