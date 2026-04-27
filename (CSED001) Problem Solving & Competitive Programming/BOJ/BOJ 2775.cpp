#include <stdio.h>

int main(){
    int T, k, n, arr[15][15];
    scanf("%d", &T);
    for(int i=0; i<T; i++){
        scanf("%d %d", &k, &n);
        for(int i=0; i<=k; i++){
            for(int j=0; j<=n; j++){
                arr[i][j]=0;
            }
        }
        for(int i=1; i<=n; i++){
            arr[0][i]=i;
        }
        for(int i=1; i<=k; i++){
            for(int j=1; j<=n; j++){
                arr[i][j]+=arr[i][j-1]+arr[i-1][j];
            }
        }
        // for(int i=0; i<=k; i++){
        //     for(int j=0; j<=n; j++){
        //         printf("%d ", arr[i][j]);
        //     }
        //     printf("\n");
        // }
        printf("%d", arr[k][n]);
        printf("\n");
    }
}