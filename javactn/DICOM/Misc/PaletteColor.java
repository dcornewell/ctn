/* Palette ColorInterface */

package DICOM.Misc;

public class PaletteColor {

  public short Entries;
  public short FirstPixel;
  public short BitsPerPixel;
  public short Map[];

  public PaletteColor() {
    Entries = 0;
    FirstPixel = 0;
    BitsPerPixel = 0;
  }

  public void dumpState()  {
    System.out.println("       Entries: " + Entries);
    System.out.println("   First Pixel: " + FirstPixel);
    System.out.println("Bits Per Pixel: " + BitsPerPixel);
  }
}
