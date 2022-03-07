int climbStairs(int n){
    int p = 0, q = 0, m = 1;
    for (int i = 1; i <= n; i++) {
        p = q;
        q = m;
        m = p + q;
    }
    return m;
}
