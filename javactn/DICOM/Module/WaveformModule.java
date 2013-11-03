/* Waveform Module Interface */

package DICOM.Module;
import DICOM.Module.WaveformChannel;

public class WaveformModule {
  public native void setChannelAttributes(long dcmObj, int ordinalNumber,
		DICOM.Module.WaveformChannel channel);
  public native void getSamplesByte(long dcmObj, byte samples[],
		int channels, int numberOfSamples);
  public native void getSamplesShort(long dcmObj, short samples[],
		int channels, int numberOfSamples);

  private int NumberOfChannels;
  private int NumberOfSamples;
  private float SamplingFrequency;
  private String GroupLabel;
  private String WaveformSampleValueRepresentation;

  //private int FrameCount;
  private long DCMObject;		// To allow access to pixel data

/*
  static {
    System.loadLibrary("javactn");
  }
*/

  public WaveformModule() {

  }

  public int getNumberOfChannels() {
    return NumberOfChannels;
  }
  public int getNumberOfSamples() {
    return NumberOfSamples;
  }
  public float getSamplingFrequency() {
    return SamplingFrequency;
  }
  public String getGroupLabel() {
    return GroupLabel;
  }
  public String getWaveformSampleValueRepresentation() {
    return WaveformSampleValueRepresentation;
  }
  public DICOM.Module.WaveformChannel getChannel(int ordinalNumber) {
    DICOM.Module.WaveformChannel channel = new DICOM.Module.WaveformChannel();
    setChannelAttributes(DCMObject, ordinalNumber, channel);
    DICOM.Misc.CodedValue v = channel.getChannelSource();
    return channel;
  }
  public void getSamples(byte samples[], int channels, int numberOfSamples) {
    getSamplesByte(DCMObject, samples, channels, numberOfSamples);
  }
  public void getSamples(short samples[], int channels, int numberOfSamples) {
    getSamplesShort(DCMObject, samples, channels, numberOfSamples);
  }
  public byte[] samplesByte(int channels, int numberOfSamples) {
    byte samples[] = new byte[channels * numberOfSamples];
    getSamplesByte(DCMObject, samples, channels, numberOfSamples);
    return samples;
  }
  public short[] samplesShort(int channels, int numberOfSamples) {
    short samples[] = new short[channels * numberOfSamples];
    getSamplesShort(DCMObject, samples, channels, numberOfSamples);
    return samples;
  }


  public void dumpState()  {
    System.out.println("  Number of Channels: " + NumberOfChannels);
    System.out.println("   Number of Samples: " + NumberOfSamples);
    System.out.println("  Sampling Frequency: " + SamplingFrequency);
    System.out.println("         Group Label: " + GroupLabel);
    System.out.println("Value Representation: " + WaveformSampleValueRepresentation);
  }
}
