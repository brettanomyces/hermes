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

    bool activateCompressor(double fzTemp);
    bool deactivateCompressor(double fzTemp);
    bool openBaffel(double frTemp);
    bool closeBaffel(double frTemp);
    bool activateHeater(double frTemp);
    bool deactivateHeater(double frTemp);
    bool activateFan(bool compressorOn, bool baffelOpen);
    bool deactivateFan(bool compressorOn, bool baffelOpen);

  private:
    double m_frSetTemp = 25;
    double m_fzSetTemp = 4;
    double m_diff = 0.25;
    bool m_frEmpty = false;
};
#endif  // SRC_TEMPERATURECONTROLLER_H_
