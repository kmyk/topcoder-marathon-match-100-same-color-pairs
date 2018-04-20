import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.security.*;
import java.util.*;
import javax.swing.*;

class Pair {
    public int r1, c1, r2, c2;
    public char col;
    public Pair(int R1, int C1, int R2, int C2, char Col) {
        r1 = R1;
        r2 = R2;
        c1 = C1;
        c2 = C2;
        col = Col;
    }
}

public class SameColorPairsVis {
    static int minS = 10, maxS = 100;
    static int minC = 2, maxC = 6;

    int H, W;

    // used for simulation / manual play
    volatile char[][] board;
    volatile int curTiles;
    java.util.List<Pair> removed;   // pairs of tiles which have been removed (for Undo)
    // -----------------------------------------
    boolean isInside(int r, int c) {
        return (r >= 0 && r < H && c >= 0 && c < W);
    }
    // -----------------------------------------
    String generate(String seedStr) {
      try {
        // generate test case
        SecureRandom r1 = SecureRandom.getInstance("SHA1PRNG"); 
        long seed = Long.parseLong(seedStr);
        r1.setSeed(seed);
        H = r1.nextInt(maxS - minS + 1) + minS;
        W = r1.nextInt(maxS - minS + 1) + minS;
        if ((W * H) % 2 == 1) {
            // make sure total number of cells on the board is even
            if (r1.nextInt(2) == 0)
                W++;
            else
                H++;
        }
        if (seed == 1)
            W = H = 10;
        else if (seed == 2)
            W = H = 30;
        else if (seed == 3)
            W = H = 100;

        int C = r1.nextInt(maxC - minC + 1) + minC;
        if (seed <= 3) {
            C = (int)seed * 2;
        }

        // generate the board: each tile has an equal probability of being each color
        board = new char[H][W];
        while (true) {
            int[] ncols = new int[C];
            for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++) {
                int c = r1.nextInt(C);
                board[i][j] = (char)((int)'0' + c);
                ncols[c]++;
            }
            // as a final fix, make sure there is even quantity of tiles of each color
            for (int i = 0; i < C; ++i)
                if (ncols[i] % 2 == 1) {
                    for (int j = i + 1; j < C; ++j)
                        if (ncols[j] % 2 == 1) {
                            // replace one tile of color j with one tile of color i
                            for (int r = 0; r < H && ncols[j] % 2 == 1; ++r)
                            for (int c = 0; c < W; ++c)
                                if (board[r][c] == (char)((int)'0' + j)) {
                                    ncols[j]--;
                                    ncols[i]++;
                                    board[r][c] = (char)((int)'0' + i);
                                    break;
                                }
                        }
                }

            // make sure that all colors are present on the board
            boolean allCols = true;
            for (int i = 0; i < C; ++i)
                allCols &= (ncols[i] > 0);
            if (allCols)
                break;
        }

        StringBuffer sb = new StringBuffer();
        sb.append("H = ").append(H).append('\n');
        sb.append("W = ").append(W).append('\n');
        sb.append("C = ").append(C).append('\n');
        for (int i = 0; i < H; ++i) {
            sb.append(new String(board[i])).append('\n');
        }
        return sb.toString();
      }
      catch (Exception e) {
        addFatalError("An exception occurred while generating test case.");
        e.printStackTrace(); 
        return "";
      }
    }
    // -----------------------------------------
    int getScore() {
        return H * W - curTiles;
    }
    // -----------------------------------------
    String removePair(int r1, int c1, int r2, int c2) {
        // validate the removal
        if (board[r1][c1] == '.' || board[r2][c2] == '.')
            return "Both tiles you're trying to remove must still be on the board.";
        if (board[r1][c1] != board[r2][c2])
            return "Both tiles you're trying to remove must have the same color.";
        if (r1 == r2 && c1 == c2)
            return "The tiles you're trying to remove must be distinct.";
        char c = board[r1][c1];
        for (int i = Math.min(r1, r2); i <= Math.max(r1, r2); ++i)
        for (int j = Math.min(c1, c2); j <= Math.max(c1, c2); ++j)
            if (board[i][j] != c && board[i][j] != '.')
                return "The rectangle specified by the tiles you're trying to remove must not contain tiles of any other color.";
        // finally, perform removal
        removed.add(new Pair(r1, c1, r2, c2, board[r1][c1]));
        board[r1][c1] = board[r2][c2] = '.';
        curTiles += 2;
        return "";
    }
    // -----------------------------------------
    public int runTest(String seed) {
      try {
        String test = generate(seed);
        if (debug)
            System.out.println(test);

        // start with zero tiles removed
        curTiles = 0;
        removed = new ArrayList<Pair>();

        if (vis) {
            jf.setSize((W+3)*SZ+50,H*SZ+40);
            jf.setVisible(true);
            manualReady = false;
            firstR = firstC = -1;
            draw();
        }

        if (proc != null) {
            // call the solution
            String[] boardArg = new String[H];
            for (int i = 0; i < H; ++i) {
                boardArg[i] = new String(board[i]);
            }

            String[] moves;
            try { 
                moves = removePairs(boardArg);
            } catch (Exception e) {
                addFatalError("Failed to get result from removePairs.");
                return H*W;
            }

            // check the return and score it
            if (moves == null) {
                addFatalError("Your return contained invalid number of elements.");
                return H*W;
            }
            if (moves.length > W*H/2) {
                addFatalError("Your return contained more than " + (W*H/2) + " elements.");
                return H*W;
            }

            // parse each move and simulate it
            // manual mode just returns 0 moves to process, because it's already processed
            for (int i = 0; i < moves.length; ++i) {
                // move format is "R1 C1 R2 C2"
                String[] s = moves[i].split(" ");
                int R1, C1, R2, C2;
                if (s.length != 4) {
                    addFatalError("Move " + i + ": Each element of your return must be formatted as \"R1 C1 R2 C2\"");
                    return H*W;
                }
                // check the cell we want to start roll from
                try {
                    R1 = Integer.parseInt(s[0]);
                    C1 = Integer.parseInt(s[1]);
                    R2 = Integer.parseInt(s[2]);
                    C2 = Integer.parseInt(s[3]);
                }
                catch (Exception e) {
                    addFatalError("Move " + i + ": All numbers in each element of your return must be integers.");
                    return H*W;
                }
                if (!isInside(R1, C1)) {
                    addFatalError("Move " + i + ": R1 and C1 in each element of your return must specify a cell within the board.");
                    return H*W;
                }
                if (!isInside(R2, C2)) {
                    addFatalError("Move " + i + ": R2 and C2 in each element of your return must specify a cell within the board.");
                    return H*W;
                }

                // try to remove the tiles
                String errmes = removePair(R1, C1, R2, C2);
                if (!errmes.equals("")) {
                    addFatalError("Move " + i + ": " + errmes);
                    return H*W;
                }

                if (vis) {
                    draw();
                }
            }
        }

        if (manual) {
            // let player know that we're waiting for their input
            addFatalError("Manual play on");

            // wait till player finishes (possibly on top of automated return)
            while (!manualReady)
            {   try { Thread.sleep(50);}
                catch (Exception e) { e.printStackTrace(); } 
            }
            // don't convert manual removals to return value, as they are already simulated
        }
        
        return getScore();
      }
      catch (Exception e) { 
        addFatalError("An exception occurred while trying to get your program's results.");
        e.printStackTrace(); 
        return H*W;
      }
    }
// ------------- visualization part ------------
    JFrame jf;
    Vis v;
    static String exec;
    static boolean vis, manual, debug;
    static Process proc;
    static int del;
    InputStream is;
    OutputStream os;
    BufferedReader br;
    static int SZ;
    volatile boolean manualReady;
    volatile boolean firstClick = true;
    volatile int firstR = -1, firstC = -1;
    // -----------------------------------------
    String[] removePairs(String[] board) throws IOException {
        StringBuffer sb = new StringBuffer();
        sb.append(H).append("\n");
        for (int i = 0; i < H; ++i) {
            sb.append(board[i]).append("\n");
        }
        os.write(sb.toString().getBytes());
        os.flush();

        // and get the return value
        int N = Integer.parseInt(br.readLine());
        String[] ret = new String[N];
        for (int i = 0; i < N; i++)
            ret[i] = br.readLine();
        return ret;
    }
    // -----------------------------------------
    void draw() {
        if (!vis) return;
        v.repaint();
        try { Thread.sleep(del); }
        catch (Exception e) { };
    }
    // -----------------------------------------
    final static int[] colors = {0x4000FF, 0xFF00BF, 0x00BFFF, 0xFFBF00, 0x40FF00, 0xBF0000};
    BufferedImage cacheBoard;
    char[][] cache;
    void DrawStartingBoard() {
        cacheBoard = new BufferedImage(W*SZ+120,H*SZ+40,BufferedImage.TYPE_INT_RGB);
        Graphics2D g2 = (Graphics2D)cacheBoard.getGraphics();
        // background
        g2.setColor(new Color(0xDDDDDD));
        g2.fillRect(0,0,W*SZ+120,H*SZ+40);
        g2.setColor(Color.WHITE);
        g2.fillRect(0,0,W*SZ,H*SZ);

        cache = new char[H][W];
        for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j) {
            cache[i][j] = board[i][j];
            if (board[i][j] != '.') {
                g2.setColor(new Color(colors[(int)(board[i][j] - '0')]));
                g2.fillRect(j * SZ + 1, i * SZ + 1, SZ - 1, SZ - 1);
            }
        }

        // lines between cells
        g2.setColor(Color.BLACK);
        for (int i = 0; i <= H; i++)
            g2.drawLine(0,i*SZ,W*SZ,i*SZ);
        for (int i = 0; i <= W; i++)
            g2.drawLine(i*SZ,0,i*SZ,H*SZ);

        g2.setFont(new Font("Arial",Font.BOLD,14));
        // "ready" "button"
        g2.drawString("READY",SZ*W+25,30);
        g2.drawRect(SZ*W+12,10,70,30);

        // "undo" "button"
        g2.drawString("UNDO",SZ*W+25,70);
        g2.drawRect(SZ*W+12,50,70,30);
    }
    static BufferedImage deepCopy(BufferedImage source) {
        BufferedImage b = new BufferedImage(source.getWidth(), source.getHeight(), source.getType());
        Graphics g = b.getGraphics();
        g.drawImage(source, 0, 0, null);
        g.dispose();
        return b;
    }
    // -----------------------------------------
    public class Vis extends JPanel implements MouseListener, WindowListener {
        public void paint(Graphics g) {
            if (cacheBoard == null) {
                DrawStartingBoard();
            }
            
            // update the cache if it is different from the current board
            Graphics2D g2 = (Graphics2D)cacheBoard.getGraphics();
            for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j) 
                if (cache[i][j] != board[i][j]) {
                    cache[i][j] = board[i][j];
                    g2.setColor(cache[i][j] == '.' ? Color.WHITE : new Color(colors[(int)(cache[i][j] - '0')]));
                    g2.fillRect(j * SZ + 1, i * SZ + 1, SZ - 1, SZ - 1);
                }
            g2.dispose();

            // copy cache to current image and add frame for selection
            BufferedImage bi = deepCopy(cacheBoard);
            g2 = (Graphics2D)bi.getGraphics();

            g2.setColor(Color.GRAY);
            if (firstR >= 0) {
                // highlight the first selected tile
                g2.setStroke(new BasicStroke(4.0f));
                g2.drawLine(firstC * SZ, firstR * SZ, (firstC + 1) * SZ, firstR * SZ);
                g2.drawLine(firstC * SZ, firstR * SZ, firstC * SZ, (firstR + 1) * SZ);
                g2.drawLine((firstC + 1) * SZ, (firstR + 1) * SZ, (firstC + 1) * SZ, firstR * SZ);
                g2.drawLine((firstC + 1) * SZ, (firstR + 1) * SZ, firstC * SZ, (firstR + 1) * SZ);
            }

            // current score
            g2.setColor(Color.BLACK);
            g2.setFont(new Font("Arial",Font.BOLD,14));
            FontMetrics fm = g2.getFontMetrics();
            g2.drawString(String.format("%d / %d", getScore(), H*W), W*SZ+25, 110+fm.getHeight());

            g.drawImage(bi,0,0,W*SZ+120,H*SZ+40,null);
        }
        // -------------------------------------
        public Vis() {
            addMouseListener(this);
            jf.addWindowListener(this);
        }
        // -------------------------------------
        //WindowListener
        public void windowClosing(WindowEvent e){ 
            if(proc != null)
                try { proc.destroy(); } 
                catch (Exception ex) { ex.printStackTrace(); }
            System.exit(0); 
        }
        public void windowActivated(WindowEvent e) { }
        public void windowDeactivated(WindowEvent e) { }
        public void windowOpened(WindowEvent e) { }
        public void windowClosed(WindowEvent e) { }
        public void windowIconified(WindowEvent e) { }
        public void windowDeiconified(WindowEvent e) { }
        // -------------------------------------
        //MouseListener
        public void mouseClicked(MouseEvent e) {
            // for manual play
            if (!manual || manualReady) return;

            // special button submits current state of the board
            int x = e.getX()-SZ*W-12, y = e.getY()-10;
            if (x >= 0 && x <= 70 && y >= 0 && y <= 30) {
                manualReady = true;
                return;
            }
            
            if (x >= 0 && x <= 70 && y >= 40 && y <= 70) {
                if (removed.isEmpty()) {
                    // nothing to undo
                    return;
                }
                // undo last removed pair
                int ind = removed.size() - 1;
                Pair last = removed.get(ind);
                removed.remove(ind);
                board[last.r1][last.c1] = board[last.r2][last.c2] = last.col;
                curTiles -= 2;
                repaint();
                return;
            }

            // regular click either stores coordinates of the tile (first click), or removes a pair of tiles (second click)

            int row = e.getY()/SZ, col = e.getX()/SZ;
            // convert to args only clicks with valid coordinates
            if (!isInside(row, col))
                return;
            // ignore clicks on empty cells
            if (board[row][col] == '.')
                return;

            if (firstClick) {
                // clicked on a tile => remember and switch to waiting for the second click
                firstR = row;
                firstC = col;
                firstClick = false;
                repaint();
                return;
            }
            // for second click, must be another tile
            // or equal to first tile, to cancel
            if (row == firstR && col == firstC) {
                // cancel selection
                firstR = firstC = -1;
                firstClick = true;
                repaint();
                return;
            }
            String errmes = removePair(firstR, firstC, row, col);
            if (!errmes.equals("")) {
                addFatalError(errmes);
                return;
            }
            // roll succeeded
            firstR = firstC = -1;
            repaint();

            firstClick = true;
            if (curTiles >= H*W-1)
                manualReady = true;
        }
        public void mousePressed(MouseEvent e) { }
        public void mouseReleased(MouseEvent e) { }
        public void mouseEntered(MouseEvent e) { }
        public void mouseExited(MouseEvent e) { }
    }
    // -----------------------------------------
    public SameColorPairsVis(String seed) {
      try {
        if (vis)
        {   jf = new JFrame();
            v = new Vis();
            jf.getContentPane().add(v);
        }
        if (exec != null) {
            try {
                Runtime rt = Runtime.getRuntime();
                proc = rt.exec(exec);
                os = proc.getOutputStream();
                is = proc.getInputStream();
                br = new BufferedReader(new InputStreamReader(is));
                new ErrorReader(proc.getErrorStream()).start();
            } catch (Exception e) { e.printStackTrace(); }
        }
        System.out.println("Score = " + runTest(seed) + " / " + (H*W));
        if (proc != null)
            try { proc.destroy(); } 
            catch (Exception e) { e.printStackTrace(); }
      }
      catch (Exception e) { e.printStackTrace(); }
    }
    // -----------------------------------------
    public static void main(String[] args) {
        String seed = "1";
        vis = true;
        manual = false;
        del = 100;
        SZ = 20;
        for (int i = 0; i<args.length; i++)
        {   if (args[i].equals("-seed"))
                seed = args[++i];
            if (args[i].equals("-exec"))
                exec = args[++i];
            if (args[i].equals("-delay"))
                del = Integer.parseInt(args[++i]);
            if (args[i].equals("-novis"))
                vis = false;
            if (args[i].equals("-manual"))
                manual = true;
            if (args[i].equals("-size"))
                SZ = Integer.parseInt(args[++i]);
            if (args[i].equals("-debug"))
                debug = true;
        }
        if (seed.equals("1"))
            SZ = 30;
        if (exec == null)
            manual = true;
        if (manual)
            vis = true;
        SameColorPairsVis f = new SameColorPairsVis(seed);
    }
    // -----------------------------------------
    void addFatalError(String message) {
        System.out.println(message);
    }
}

class ErrorReader extends Thread{
    InputStream error;
    public ErrorReader(InputStream is) {
        error = is;
    }
    public void run() {
        try {
            byte[] ch = new byte[50000];
            int read;
            while ((read = error.read(ch)) > 0)
            {   String s = new String(ch,0,read);
                System.out.print(s);
                System.out.flush();
            }
        } catch(Exception e) { }
    }
}
