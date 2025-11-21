#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// SCS 하나 출력: LCS 길이 테이블을 만들고, 그 정보를 이용해 SCS 복원
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b;
    while (true) {
        if (!(cin >> a)) break;        // 첫 문자열 실패 → EOF
        if (!(cin >> b)) break;        // 두 번째 문자열 실패 → EOF

        int n = (int)a.size(), m = (int)b.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0)); // dp[i][j]=LCS(a[i:], b[j:])

        // 뒤에서 앞으로 LCS 길이 테이블
        for (int i = n - 1; i >= 0; --i) {
            for (int j = m - 1; j >= 0; --j) {
                if (a[i] == b[j]) dp[i][j] = dp[i + 1][j + 1] + 1;
                else dp[i][j] = max(dp[i + 1][j], dp[i][j + 1]);
            }
        }

        // SCS 복원
        string ans;
        int i = 0, j = 0;
        while (i < n && j < m) {
            if (a[i] == b[j]) {
                ans.push_back(a[i]); ++i; ++j;
            } else if (dp[i + 1][j] >= dp[i][j + 1]) {
                ans.push_back(a[i]); ++i;
            } else {
                ans.push_back(b[j]); ++j;
            }
        }
        while (i < n) ans.push_back(a[i++]);
        while (j < m) ans.push_back(b[j++]);

        cout << ans << '\n';
    }
    return 0;
}
