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

    bool shouldActivateCompressor(double fzTemp, bool isWaiting);
    bool shouldDeactivateCompressor(double fzTemp, bool isWaiting);
    bool shouldOpenBaffel(double frTemp);
    bool shouldCloseBaffel(double frTemp);
    bool shouldActivateHeater(double frTemp, bool isWaiting);
    bool shouldDeactivateHeater(double frTemp, bool isWaiting);
    bool shouldActivateFan(bool compressorOn, bool baffelOpen);
    bool shouldDeactivateFan(bool compressorOn, bool baffelOpen);

  private:
    double m_frSetTemp = 25;
    double m_fzSetTemp = 4;
    double m_diff = 0.25;
    bool m_frEmpty = false;
};
#endif  // SRC_TEMPERATURECONTROLLER_H_
