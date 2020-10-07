
String secondsToHMS(unsigned int seconds) {
    int t = seconds;
    
    int m = 0;
    int s = 0;
    String S = "";
    String M = "";

    s = t % 60;
    m = (t - s) / 60;

    if (s < 10) {
      S = "0" + String(s);
    } else {
      S = String(s);
    }
    if (m < 10) {
      M = "0" + String(m);
    } else {
      M = String(m);
    }
                         
    return String(M) + ":" + String(S);
}
