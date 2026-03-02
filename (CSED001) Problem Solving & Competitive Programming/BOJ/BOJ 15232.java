// BOJ 15232: Rectangles

import java.io.*;

class BOJ_15232 {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out));

        int R = Integer.parseInt(br.readLine().trim());
        int C = Integer.parseInt(br.readLine().trim());
        for(int i=1; i<=R; ++i){
            for(int j=1; j<=C; ++j){
                bw.write('*');
            }
            bw.newLine();
        }
        bw.flush();
    }
}