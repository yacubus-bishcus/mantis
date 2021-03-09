void ZTest(double c1, double c2)
{
  double zscore = abs(c1 - c2)/(sqrt(pow(sqrt(c1),2) + pow(sqrt(c2),2)));
  std::cout << "The Z-Score: " << zscore << std::endl;
}
