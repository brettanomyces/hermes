#ifndef SRC_TEMPERATURECONTROLLER_H_
#define SRC_TEMPERATURECONTROLLER_H_

class TemperatureController {
  public:
    TemperatureController(); 

    void setFzSetTemp(double temp);
    double getFzSetTemp();
    void setFrSetTemp(double temp);
    double getFrSetTemp();
    void setDifference(double degrees);
    double getDifference();

    bool toggleCompressor(bool compressorOn, double fzTemp);
    bool toggleBaffel(bool baffelOpen, double frTemp);
    bool toggleHeater(bool heaterOn, double frTemp);
    bool toggleFan(bool fanOn, bool compressorOn, bool baffelOpen);

  private:
    double m_frSetTemp = 25;
    double m_fzSetTemp = 4;
    double m_diff = 0.25;
    bool m_frEmpty = false;
};
#endif  // SRC_TEMPERATURECONTROLLER_H_
