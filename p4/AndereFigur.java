public class AndereFigur extends Figur {
    public void setPosition(char x, int y) {
        this.x = x;
        MachMal.eineZehntelSekundeLangGarNichts();
        this.y = y;
    }

    public String getPosition() {
        MachMal.eineSekundeLangGarNichts();
        return "(" + x + ", " + y + ")";
    }
}
