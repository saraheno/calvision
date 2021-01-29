
void plot_Eion()
{
   //-------------------------
   // examples of making plots
   //-------------------------

   TFile *ffile = new TFile("test.root"); //use you own root file
   TTree *tree;
   ffile->GetObject("tree", tree);

   // get TBranches
   Float_t depositedIonEnergyTotal;
   Float_t proton_EIon;
   Float_t Ninelastic;
   TBranch *b_depositedIonEnergyTotal;
   TBranch *b_proton_EIon;
   TBranch *b_Ninelastic;
   tree->SetBranchAddress("depositedIonEnergyTotal", &depositedIonEnergyTotal, &b_depositedIonEnergyTotal);
   tree->SetBranchAddress("proton_EIon", &proton_EIon, &b_proton_EIon);
   tree->SetBranchAddress("Ninelastic", &Ninelastic, &b_Ninelastic);

   double total_ion = 0;
   TH2F *h_Ninelas_Ep = new TH2F("h_Ninelas_Ep", ";Proton Energy deposition (GeV);Number of inelastic interactions", 100, 0, 5, 100, 0, 600);

   // loop all the events
   for (Long64_t jentry = 0; jentry < tree->GetEntries(); jentry++)
   {
      tree->GetEntry(jentry);
      h_Ninelas_Ep->Fill(proton_EIon, Ninelastic);
      total_ion += depositedIonEnergyTotal;
   }
   cout << "average ion:" << total_ion / tree->GetEntries() << endl;
   cout << "average ion in hist:" << ((TH2D *)ffile->Get("h_EIon_beta"))->Integral() / tree->GetEntries() << endl;

   // pdgid: https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf
   TH1D *hpi_p = (TH1D *)(((TH2D *)ffile->Get("h_EIon_beta"))->ProjectionY("", 5211, 5212))->Clone();
   TH1D *hpi_m = (TH1D *)(((TH2D *)ffile->Get("h_EIon_beta"))->ProjectionY("", 4789, 4791))->Clone();
   TH1D *hpi0 = (TH1D *)(((TH2D *)ffile->Get("h_EIon_beta"))->ProjectionY("", 5111, 5113))->Clone();
   TH1D *he_p = (TH1D *)(((TH2D *)ffile->Get("h_EIon_beta"))->ProjectionY("", 4990, 4990))->Clone();
   TH1D *he_m = (TH1D *)(((TH2D *)ffile->Get("h_EIon_beta"))->ProjectionY("", 5012, 5012))->Clone();
   TH1D *hp = (TH1D *)(((TH2D *)ffile->Get("h_EIon_beta"))->ProjectionY("", 7212, 7214))->Clone();
   TH1D *hn = (TH1D *)(((TH2D *)ffile->Get("h_EIon_beta"))->ProjectionY("", 7112, 7114))->Clone();

   TCanvas *c1 = new TCanvas();
   c1->SetLogy();
   gStyle->SetOptStat(0000);
   gStyle->SetOptTitle(0);

   hpi_p->Draw("HIST");
   hpi_m->Draw("same HIST");
   hpi0->Draw("same HIST");
   he_p->Draw("same HIST");
   he_m->Draw("same HIST");
   hp->Draw("same HIST");
   hn->Draw("same HIST");

   hpi_p->SetTitle("pi+;#beta;fraction of ionizing energy");
   hpi_m->SetTitle("pi-");
   hpi0->SetTitle("pi0");
   he_p->SetTitle("e+");
   he_m->SetTitle("e-");
   hp->SetTitle("proton");
   hn->SetTitle("neutron");

   hpi_p->SetLineColor(kBlack);
   hpi_m->SetLineColor(kBlue);
   hpi0->SetLineColor(kRed);
   he_p->SetLineColor(kGreen);
   he_m->SetLineColor(kCyan);
   hp->SetLineColor(kOrange);
   hn->SetLineColor(kGray);

   hpi_p->SetLineWidth(3);
   hpi_m->SetLineWidth(3);
   hpi0->SetLineWidth(3);
   he_p->SetLineWidth(3);
   he_m->SetLineWidth(3);
   hp->SetLineWidth(3);
   hn->SetLineWidth(3);

   hpi_p->Scale(1.0 / total_ion);
   hpi_m->Scale(1.0 / total_ion);
   hpi0->Scale(1.0 / total_ion);
   he_p->Scale(1.0 / total_ion);
   he_m->Scale(1.0 / total_ion);
   hp->Scale(1.0 / total_ion);
   hn->Scale(1.0 / total_ion);

   hpi_p->GetYaxis()->SetRangeUser(0.001, 0.5);
   hpi_p->GetXaxis()->SetRangeUser(0, 1);

   auto leg = new TLegend();
   leg = c1->BuildLegend();
   leg->SetLineWidth(0);
   leg->SetTextSize(0.03);

   TPaveText *t = new TPaveText(0.35, 0.9, 0.6, 1.0, "brNDC"); // left-up
   t->AddText("ionizing energy");
   t->SetTextSize(0.05);
   t->SetBorderSize(0);
   t->SetFillColor(0);
   t->SetFillStyle(0);
   t->SetTextFont(42);
   t->Draw();

   TCanvas *c2 = new TCanvas();
   h_Ninelas_Ep->Draw("colz");
}
