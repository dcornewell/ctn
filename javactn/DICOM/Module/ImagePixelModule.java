/* Image Pixel Module Interface */

package DICOM.Module;
import DICOM.Misc.PaletteColor;

public class ImagePixelModule {
  public native void getPixelsByte(long dcmObj, byte pixels[], int rows,
				   int cols, int samplesPerPixel);
  public native void getPixelsShort(long dcmObj, short pixels[], int rows,
				   int cols, int samplesPerPixel);
  public native void remapPixelsByte(long dcmObj, byte pixelsIn[], int rows,
				   int cols, int samplesPerPixel,
				   int pixelsOut[]);
  public native void getPixelFrameByte(long dcmObj, byte pixels[],
		int rows, int cols, int samplesPerPixel, int frame);
  public native void getPixelFrameShort(long dcmObj, short pixels[],
		int rows, int cols, int samplesPerPixel, int frame);
  public native void  getPixelFrameAsColor(long dcmObj, int pixels[],
			int rows, int cols, int frame);
  public native void  getPixelFrameAsIndexed(long dcmObj, byte pixels[],
			int rows, int cols, int frame);
  public native void fillPaletteDescriptor(long dcmObj, String color,
		DICOM.Misc.PaletteColor palette);
  public native void fillPaletteMap(long dcmObj, String color, int entries,
		int bitsPerPixel, short pixels[]);

  private int SamplesPerPixel;
  private String PhotometricInterpretation;
  private int Rows;
  private int Columns;
  private int BitsAllocated;
  private int BitsStored;
  private int HighBit;
  private int PixelRepresentation;
  private float WindowCenter;
  private float WindowWidth;
  private float RescaleSlope;
  private float RescaleIntercept;

  // Really is defined in another module, but we will define it here for now
  private int FrameCount;
  private long DCMObject;		// To allow access to pixel data

  public ImagePixelModule() {

  }

  public int getSamplesPerPixel() {
    return SamplesPerPixel;
  }
  public String getPhotometricInterpretation() {
    return PhotometricInterpretation;
  }
  public int getRows() {
    return Rows;
  }
  public int getColumns() {
    return Columns;
  }
  public int getBitsAllocated() {
    return BitsAllocated;
  }
  public int getBitsStored() {
    return BitsStored;
  }
  public int getHighBit() {
    return HighBit;
  }
  public int getPixelRepresentation() {
    return PixelRepresentation;
  }
  public float getWindowCenter() {
    return WindowCenter;
  }
  public float getWindowWidth() {
    return WindowWidth;
  }
  public float getRescaleSlope() {
    return RescaleSlope;
  }
  public float getRescaleIntercept() {
    return RescaleIntercept;
  }
  public void getPixels(byte pixels[], int rows, int cols) {
    getPixelsByte(DCMObject, pixels, rows, cols, SamplesPerPixel);
  }
  public void getPixels(short pixels[], int rows, int cols) {
    getPixelsShort(DCMObject, pixels, rows, cols, SamplesPerPixel);
  }
  public int getFrameCount() {
    return FrameCount;
  }
  public void getPixelFrame(byte pixels[], int rows, int cols, int frame) {
    getPixelFrameByte(DCMObject, pixels, rows, cols, SamplesPerPixel, frame);
  }
  public void getPixelFrame(short pixels[], int rows, int cols, int frame) {
    getPixelFrameShort(DCMObject, pixels, rows, cols, SamplesPerPixel, frame);
  }
  public void getFrameAsColor(int pixels[], int rows, int cols, int frame) {
   getPixelFrameAsColor(DCMObject, pixels, rows, cols, frame);
  }
  public void getFrameAsIndexed(byte pixels[], int rows, int cols, int frame) {
   getPixelFrameAsIndexed(DCMObject, pixels, rows, cols, frame);
  }
  public int[] ColorFrame(int rows, int cols, int index) {
    int frame[] = new int[rows * cols];
    getPixelFrameAsColor(DCMObject, frame, rows, cols, index);
    return frame;
  }

  public DICOM.Misc.PaletteColor getPalette(String color) {
    DICOM.Misc.PaletteColor palette = new DICOM.Misc.PaletteColor();

     fillPaletteDescriptor(DCMObject, color, palette);
     palette.Map = new short[palette.Entries];
     fillPaletteMap(DCMObject, color, palette.Entries, palette.BitsPerPixel,
		    palette.Map);

    return palette;
  }
  public void remapPixels(byte pixelsIn[], int rows, int cols, int pixelsOut[]){
    remapPixelsByte(DCMObject, pixelsIn, rows, cols, SamplesPerPixel, pixelsOut);
  }

  public void dumpState()  {
    System.out.println("   Samples Per Pixel: " + SamplesPerPixel);
    System.out.println("  Photometric Interp: " + PhotometricInterpretation);
    System.out.println("                Rows: " + Rows);
    System.out.println("             Columns: " + Columns);
    System.out.println("      Bits Allocated: " + BitsAllocated);
    System.out.println("         Bits Stored: " + BitsStored);
    System.out.println("            High Bit: " + HighBit);
    System.out.println("Pixel Representation: " + PixelRepresentation);
    System.out.println("       Window Center: " + WindowCenter);
    System.out.println("        Window Width: " + WindowWidth);
    System.out.println("       Rescale Slope: " + RescaleSlope);
    System.out.println("   Rescale Intercept: " + RescaleIntercept);
    System.out.println("         Frame Count: " + FrameCount);
  }
}
