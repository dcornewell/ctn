/* Waveform Channel Interface */

package DICOM.Module;
import DICOM.Misc.CodedValue;

public class WaveformChannel {

  private int ChannelNumber;
  private String ChannelLabel;
  private String ChannelStatus;
  private float ChannelSensitivity;
  private float ChannelSensitivityCorrectionFactor;
  private float ChannelBaseline;
  private int BitsPerSample;
  private DICOM.Misc.CodedValue ChannelSource;
  private DICOM.Misc.CodedValue ChannelSensitivityUnits;

  public WaveformChannel() {
    ChannelSource = new DICOM.Misc.CodedValue();
    ChannelSensitivityUnits = new DICOM.Misc.CodedValue();
  }

  public int getChannelNumber() {
    return ChannelNumber;
  }
  public String getChannelLabel() {
    return ChannelLabel;
  }
  public String getChannelStatus() {
    return ChannelStatus;
  }
  public float getChannelSensitivity() {
    return ChannelSensitivity;
  }
  public float getChannelSensitivityCorrectionFactor() {
    return ChannelSensitivityCorrectionFactor;
  }
  public float getChannelBaseline() {
    return ChannelBaseline;
  }
  public int getBitsPerSample() {
    return BitsPerSample;
  }
  public DICOM.Misc.CodedValue getChannelSource() {
    return ChannelSource;
  }
  public DICOM.Misc.CodedValue getChannelSensitivityUnits() {
    return ChannelSensitivityUnits;
  }

  public void dumpState()  {
    System.out.println("      Channel Number: " + ChannelNumber);
    System.out.println("       Channel Label: " + ChannelLabel);
    System.out.println("      Channel Status: " + ChannelStatus);
    System.out.println(" Channel Sensitivity: " + ChannelSensitivity);
    System.out.println("     Sens Correction: " + ChannelSensitivityCorrectionFactor);
    System.out.println("    Channel Baseline: " + ChannelBaseline);
    System.out.println("     Bits Per Sample: " + BitsPerSample);
    System.out.println("      Channel Source:");
    ChannelSource.dumpState();
    System.out.println("   Sensitivity Units:");
    ChannelSensitivityUnits.dumpState();
  }
}
