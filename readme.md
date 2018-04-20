# TopCoder Marathon Match 100: SameColorPairs

<https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=17143&pm=14889>

## 記録

### 問題概要

H * Wの盤面があり各マスはC種の色のうちどれかで塗られている。
同じ色のマスの対で、それらを含む軸平行長方形内に他の色が存在しないようなものがあれば、それらのマスを無色にできる。
適当な操作列を取り無色のマスの数をできる限り大きくせよ。
(10 ≦ H, W ≦ 100, 2 ≦ C ≦ 6)

### 4/19

問題を読んだ。
初手でそれなりのが出てそこから縮まなさそうだなあという印象。
焼き鈍したいけど文脈が入ってて難しそう。
algoの問題として出てたらフローな気がする。

とりあえず愚直を書いた: `d80706d423b71e45d9acedc05f434216559d5580`。
盤面が大きくて色が少ないのが簡単 (例: `seed = 1000`) で、小さくて色が多いのが難しい (例: `seed = 1004`) ようだ。
難しいのは最後の詰めの部分だけなので最適解狙っていけるのでは。
荒く削って残りをSAT solverに投げたい。

gif出力機能はなぜ配られなかったのか。上位者の出力を眺めてもなぜか最終的にすっきり消えるということしか分からなさそうだが。

初提出は `d80706d423b71e45d9acedc05f434216559d5580` で5位。1日目にしては低めの順位か。

| Handle          | Score           | Rank    | Last Submission Time      | Language  | Example Tests   | Submissions     | Current Rating  | Predicted Rating   | Rating Delta |
|-----------------|-----------------|---------|---------------------------|-----------|-----------------|-----------------|-----------------|--------------------|--------------|
| yowa            | 982058.82       | 1       | 04.19.2018 00:39:05       | C++       | 2               | 2               | 2157            | 2232.77            | 75.77        |
| CatalinT        | 856764.71       | 2       | 04.19.2018 02:53:37       | C++       | 1               | 1               | 2504            | 2504.95            | 0.95         |
| iehn            | 840000.00       | 3       | 04.18.2018 22:19:55       | C++       | 1               | 1               | 1650            | 1758.33            | 108.33       |
| Rustyoldman     | 803529.41       | 4       | 04.19.2018 02:40:42       | C++       | 1               | 2               | 1609            | 1672.40            | 63.40        |
| kimiyuki        | 800294.12       | 5       | 04.19.2018 03:03:27       | C++       | 1               | 1               | 1745            | 1781.11            | 36.11        |
| mmammel         | 742058.82       | 6       | 04.18.2018 23:35:37       | C++       | 1               | 1               | 1292            | 1375.85            | 83.85        |
| accel           | 625882.35       | 7       | 04.19.2018 02:31:05       | C++       | 3               | 2               | 1220            | 1461.28            | 241.28       |


時間計測がなんだか変でExample Testsが増えた。まだちゃんとできてないが提出 `cb164952586698062c717fd6584600c2585b9897`。
[Pag2](https://www.topcoder.com/members/Pag2/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんが1位に出てるが青色なので私にも辿り着ける解法だろう。

| Handle        | Score        |  Rank   |  Last Submission Time | Language |  Example Tests | Submissions   | Current Rating |  Predicted Rating |  Rating Delta |
|---------------|--------------|---------|-----------------------|----------|----------------|---------------|----------------|-------------------|---------------|
| Pag2          | 974782.61    |  1      |  04.19.2018 04:03:32  | C++      |  1             | 1             | 1399           |  1576.27          |  177.27       |
| CatalinT      | 921086.96    |  2      |  04.19.2018 04:55:27  | C++      |  3             | 2             | 2504           |  2506.37          |  2.37         |
| yowa          | 911739.13    |  3      |  04.19.2018 00:39:05  | C++      |  2             | 2             | 2157           |  2172.17          |  15.17        |
| kimiyuki      | 904565.22    |  4      |  04.19.2018 05:04:49  | C++      |  8             | 2             | 1745           |  1821.92          |  76.92        |
| iehn          | 789565.22    |  5      |  04.18.2018 22:19:55  | C++      |  1             | 1             | 1650           |  1712.70          |  62.70        |
| Rustyoldman   | 769130.43    |  6      |  04.19.2018 02:40:42  | C++      |  1             | 2             | 1609           |  1643.27          |  34.27        |

次にやるべきは適当なところまで進めて残りを乱択する高速化に思える。
下から貪欲に取って座標圧縮とかをするとよさそう。
ある種の縮退したビームサーチか。

### 4/19 24:00 ~

緑が1位になってる。まあはい。

| Handle       | Score      | Rank | Last Submission Time    | Language   | Example Tests     | Submissions       | Current Rating    | Predicted Rating  | Rating Delta |
|--------------|------------|------|-------------------------|------------|-------------------|-------------------|-------------------|-------------------|--------------|
| ceni1055     | 952627.12  | 1    | 04.19.2018 11:50:49     | C++        | 3                 | 4                 | 1009              | 1659.00           | 650.00       |
| Pag2         | 952627.12  | 1    | 04.19.2018 04:03:32     | C++        | 1                 | 1                 | 1399              | 1576.27           | 177.27       |
| ebicochineal | 933135.59  | 3    | 04.19.2018 11:09:55     | C++        | 1                 | 2                 | 1623              | 1820.31           | 197.31       |
| kurenai3110  | 930508.47  | 4    | 04.19.2018 10:23:35     | C++        | 2                 | 2                 | 2137              | 2190.03           | 53.03        |
| yowa         | 923135.59  | 5    | 04.19.2018 12:11:38     | C++        | 3                 | 3                 | 2157              | 2189.10           | 32.10        |
| CatalinT     | 893220.34  | 6    | 04.19.2018 04:55:27     | C++        | 3                 | 2                 | 2504              | 2477.67           | -26.33       |
| dimkadimon   | 884237.29  | 7    | 04.19.2018 08:00:41     | Java       | 2                 | 1                 | 1646              | 1739.77           | 93.77        |
| eris_c       | 871525.42  | 8    | 04.19.2018 11:38:48     | C++        | 2                 | 2                 | 1514              | 1841.79           | 327.79       |
| kimiyuki     | 870254.24  | 9    | 04.19.2018 05:04:49     | C++        | 9                 | 2                 | 1745              | 1830.07           | 85.07        |
| MaxBuzz      | 865254.24  | 10   | 04.19.2018 11:25:18     | Java       | 3                 | 2                 | 1393              | 1523.64           | 130.64       |

rejudgeがあった。[ceni1055](https://www.topcoder.com/members/ceni1055/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さん [Pag2](https://www.topcoder.com/members/Pag2/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんが同点1位だったがtesterのバグを突いてたようで共に0点に落ちていた。

`33ec0a8eb9474d17d67cb7ec7851e337ded45d0f` を提出。5位。
外側のものは残りやすいのでそれらを先に片付けて少数だけ残して決め打ちし、それら残りを集中的に乱択で処理する。

[kurenai3110](https://www.topcoder.com/members/kurenai3110/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さん [yowa](https://www.topcoder.com/members/yowa/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんが上にいる。まだ初日深夜だが彼らいつもこの時間にこんな順位なんだっけかという気持ち。
[shamai](https://www.topcoder.com/members/shamal/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さん [ebicochineal](https://www.topcoder.com/members/ebicochineal/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんがさらに上にいる。共にratingが未収束でなんとも言えないね。

| Handle        | Score       | Rank   | Last Submission Time      | Language    | Example Tests     | Submissions | Current Rating    | Predicted Rating  | Rating Delta |
|---------------|-------------|--------|---------------------------|-------------|-------------------|-------------|-------------------|-------------------|--------------|
| shamal        | 953560.61   | 1      | 04.19.2018 13:44:19       | C++         | 6                 | 3           | 1627              | 1901.85           | 274.85       |
| ebicochineal  | 942348.48   | 2      | 04.19.2018 13:10:29       | C++         | 1                 | 3           | 1623              | 1864.42           | 241.42       |
| kurenai3110   | 932500.00   | 3      | 04.19.2018 10:23:35       | C++         | 2                 | 2           | 2137              | 2213.70           | 76.70        |
| yowa          | 928560.61   | 4      | 04.19.2018 12:11:38       | C++         | 3                 | 3           | 2157              | 2205.63           | 48.63        |
| kimiyuki      | 923484.85   | 5      | 04.19.2018 14:00:26       | C++         | 11                | 3           | 1745              | 1896.23           | 151.23       |
| CatalinT      | 893636.36   | 6      | 04.19.2018 04:55:27       | C++         | 3                 | 2           | 2504              | 2480.71           | -23.29       |

次やるべきことは焼き鈍しに思える。
対の列と見ると焼けないが、対の集合から列の復元が実質的に一意であることを考えると焼ける。

### 4/19

対の集合から列の復元は単純には集合の大きさ k に対して O(k^2) 回の対の有効性判定。
愚直に停止するまでloopしても依存関係のグラフ作っても計算量としては変わらない。
盤面上に乗っているなどの対の性質や失敗したとき継続を作って判定再開可能だとかの有効性判定述語の性質を利用すれば実質 O(k) に落ちる。
O(k^2) 回の判定再開は発生するがこれは軽いので無視できる。

焼き鈍しでなくてもっと雑な修正山登りのようなものをした。いい感じに見えるけど悪化してるケースもあるっぽい？
そういえば前処理の後に座圧しておきたい。

色数が多い場合 C = 6 は初手で書いた愚直が強いっぽい。適当に分岐入れればいいのかな。改善がしにくくなるからまだensembleはしたくない気がするがここまで差があるとしかたがなさそう。

```
ef25d3886933e2460338d966f54db435e0213b88
1) 1.0
2) 1.0
3) 0.9688
4) 0.958758109360519
5) 0.9979919678714859
6) 1.0
7) 0.999244142101285
8) 0.9972789115646259
9) 1.0
10) 1.0
```

```
958cf1fba027bb94691c892443888a77bc1c9a41
1) 1.0
2) 1.0
3) 0.9108
4) 0.8220574606116775
5) 1.0
6) 1.0
7) 1.0
8) 1.0
9) 1.0
10) 1.0
```

そもそも円形にする前処理はどのくらい妥当なんだったっけ？

ensemble `5d9cb5c536e63d2122ac8c96f48db7e10556e2d5` 提出。同状況で 851188.12 -> 862970.30 の微増。渋くない？

|  Handle         | Score          | Rank   | Last Submission Time     | Language     | Example Tests  | Submissions   |
|-----------------|----------------|--------|--------------------------|--------------|----------------|---------------|
|  sdya           | 921683.17      | 1      | 04.19.2018 17:11:35      | C++          | 1              | 1             |
|  shamal         | 909257.43      | 2      | 04.19.2018 13:44:19      | C++          | 6              | 3             |
|  CatalinT       | 906336.63      | 3      | 04.20.2018 04:06:54      | C++          | 4              | 4             |
|  shilov         | 904851.49      | 4      | 04.20.2018 00:57:03      | Java         | 9              | 4             |
|  ebicochineal   | 900792.08      | 5      | 04.20.2018 00:17:27      | C++          | 1              | 5             |
|  iwashi31       | 894009.90      | 6      | 04.19.2018 19:47:30      | C++          | 2              | 2             |
|  gardn999       | 885594.06      | 7      | 04.20.2018 00:06:10      | Java         | 4              | 4             |
|  kurenai3110    | 884059.41      | 8      | 04.20.2018 00:40:08      | C++          | 3              | 3             |
|  JacoCronje     | 882326.73      | 9      | 04.19.2018 15:15:21      | C++          | 1              | 1             |
|  dimkadimon     | 868316.83      | 10     | 04.20.2018 02:26:05      | Java         | 4              | 3             |
|  cannab         | 866732.67      | 11     | 04.20.2018 01:51:06      | C++          | 2              | 2             |
|  ki-ki          | 865841.58      | 12     | 04.19.2018 20:35:18      | Java         | 3              | 3             |
|  kimiyuki       | 862970.30      | 13     | 04.20.2018 04:45:03      | C++          | 15             | 4             |

rating predictorが壊れてたので直した。これ [#2](https://github.com/kmyk/topcoder-marathon-match-rating-predictor/issues/2) ひどくない？

消去可能判定のために双方向linked listを置いておくのはどうか。bitsetの方が良いかもだが。

手で遊んでみたが何も分からない。明らかに手動だと点数が出ない。
上下に隣接してたりするやつを優先的に消すとか、自明に消せるやつは置いておいて他の邪魔をしたときに始めて消すとか、そういうのがいい気がするがこれは人間固有な気もする。

末尾を消すのでなくて適当に間引くのはどうか。
対の集合からの列復元でしか山登りをしてないが、貪欲でも同様にできるのでは。

ensemble `5d9cb5c536e63d2122ac8c96f48db7e10556e2d5` で100個。
C = 2,3,4 はすべて満点を取れている。
C = 5 では満点は16個中4個。
C = 6 では全滅。

| seed  | C | score  |
|-------|---|--------|
| 1     | 2 | 1.0    |
| 2     | 4 | 1.0    |
| 3     | 6 | 0.9908 |
| 4     | 6 | 0.9782 |
| 5     | 4 | 1.0    |
| 6     | 4 | 1.0    |
| 7     | 3 | 1.0    |
| 8     | 4 | 1.0    |
| 9     | 3 | 1.0    |
| 10    | 2 | 1.0    |
| 11    | 5 | 0.9855 |
| 12    | 3 | 1.0    |
| 13    | 6 | 0.9875 |
| 14    | 6 | 0.9659 |
| 15    | 3 | 1.0    |
| 16    | 4 | 1.0    |
| 17    | 5 | 0.9777 |
| 18    | 5 | 0.9966 |
| 19    | 6 | 0.9306 |
| 20    | 2 | 1.0    |
| 21    | 3 | 1.0    |
| 22    | 6 | 0.9842 |
| 23    | 2 | 1.0    |
| 24    | 6 | 0.9753 |
| 25    | 3 | 1.0    |
| 26    | 5 | 1.0    |
| 27    | 4 | 1.0    |
| 28    | 2 | 1.0    |
| 29    | 2 | 1.0    |
| 30    | 5 | 0.9975 |
| 31    | 6 | 0.9925 |
| 32    | 4 | 1.0    |
| 33    | 6 | 0.9822 |
| 34    | 4 | 1.0    |
| 35    | 3 | 1.0    |
| 36    | 2 | 1.0    |
| 37    | 6 | 0.9801 |
| 38    | 4 | 1.0    |
| 39    | 3 | 1.0    |
| 40    | 3 | 1.0    |
| 41    | 2 | 1.0    |
| 42    | 3 | 1.0    |
| 43    | 6 | 0.9883 |
| 44    | 2 | 1.0    |
| 45    | 4 | 1.0    |
| 46    | 2 | 1.0    |
| 47    | 5 | 1.0    |
| 48    | 5 | 1.0    |
| 49    | 2 | 1.0    |
| 50    | 6 | 0.9875 |
| 51    | 3 | 1.0    |
| 52    | 2 | 1.0    |
| 53    | 6 | 0.9833 |
| 54    | 3 | 1.0    |
| 55    | 3 | 1.0    |
| 56    | 4 | 1.0    |
| 57    | 6 | 0.9647 |
| 58    | 6 | 0.9592 |
| 59    | 2 | 1.0    |
| 60    | 3 | 1.0    |
| 61    | 2 | 1.0    |
| 62    | 5 | 0.9927 |
| 63    | 4 | 1.0    |
| 64    | 5 | 1.0    |
| 65    | 3 | 1.0    |
| 66    | 4 | 1.0    |
| 67    | 2 | 1.0    |
| 68    | 5 | 0.9780 |
| 69    | 4 | 1.0    |
| 70    | 5 | 0.9961 |
| 71    | 4 | 1.0    |
| 72    | 5 | 0.9852 |
| 73    | 5 | 0.9984 |
| 74    | 2 | 1.0    |
| 75    | 3 | 1.0    |
| 76    | 3 | 1.0    |
| 77    | 3 | 1.0    |
| 78    | 4 | 1.0    |
| 79    | 2 | 1.0    |
| 80    | 2 | 1.0    |
| 81    | 5 | 0.9986 |
| 82    | 6 | 0.9823 |
| 83    | 3 | 1.0    |
| 84    | 2 | 1.0    |
| 85    | 3 | 1.0    |
| 86    | 4 | 1.0    |
| 87    | 4 | 1.0    |
| 88    | 5 | 0.9949 |
| 89    | 3 | 1.0    |
| 90    | 4 | 1.0    |
| 91    | 6 | 0.9894 |
| 92    | 2 | 1.0    |
| 93    | 2 | 1.0    |
| 94    | 4 | 1.0    |
| 95    | 3 | 1.0    |
| 96    | 3 | 1.0    |
| 97    | 3 | 1.0    |
| 98    | 4 | 1.0    |
| 99    | 6 | 0.9833 |
| 100   | 5 | 0.9946 |


変なことするの止めて貪欲と修正の繰り返しにしたら良くなった。
`06df28f97a648764adfd1e1aff41f4acaa9a6393` 提出。
暫定2位で驚き。
完全にラッキーパンチ。よく分からないまま上がったということは後はいつものように下がるだけになるので困る。
この単純さだとどうせみんな辿り着くよなあという気持ち。コードが短かくなったのは嬉しいが。

| Handle      | Score     | Rank      | Last Submission Time  | Language  | Example Tests   | Submissions     | Current Rating  | Predicted Rating      | Rating Delta |
|-------------|-----------|-----------|-----------------------|-----------|-----------------|-----------------|-----------------|-----------------------|--------------|
| sdya        | 918782.61 | 1         | 04.19.2018 17:11:35   | C++       | 1               | 1               | 2163            | 2317.20               | 154.20       |
| kimiyuki    | 909260.87 | 2         | 04.20.2018 08:54:18   | C++       | 16              | 5               | 1745            | 2021.32               | 276.32       |
| shamal      | 903304.35 | 3         | 04.19.2018 13:44:19   | C++       | 6               | 3               | 1627            | 1856.56               | 229.56       |
| shilov      | 900826.09 | 4         | 04.20.2018 00:57:03   | Java      | 9               | 4               | 985             | 1190.56               | 205.56       |
| CatalinT    | 900695.65 | 5         | 04.20.2018 04:06:54   | C++       | 4               | 4               | 2504            | 2511.47               | 7.47         |

C = 5 の場合もすべて満点が出ている。C = 2,3,4 の場合も満点なので省略している。
最終的に全完もありえるように思えてきた。

| seed  | C | score  |
|-------|---|--------|
| 3     | 6 | 0.9962 |
| 4     | 6 | 0.9898 |
| 11    | 5 | 1.0    |
| 13    | 6 | 0.9951 |
| 14    | 6 | 1.0    |
| 17    | 5 | 1.0    |
| 18    | 5 | 1.0    |
| 19    | 6 | 0.9760 |
| 22    | 6 | 0.9931 |
| 24    | 6 | 0.9942 |
| 26    | 5 | 1.0    |
| 30    | 5 | 1.0    |
| 31    | 6 | 0.9922 |
| 33    | 6 | 0.9921 |
| 37    | 6 | 0.9928 |
| 43    | 6 | 0.9955 |
| 47    | 5 | 1.0    |
| 48    | 5 | 1.0    |
| 50    | 6 | 0.9886 |
| 53    | 6 | 0.9966 |
| 57    | 6 | 0.9898 |
| 58    | 6 | 0.9905 |
| 62    | 5 | 1.0    |
| 64    | 5 | 1.0    |
| 68    | 5 | 1.0    |
| 70    | 5 | 1.0    |
| 72    | 5 | 1.0    |
| 73    | 5 | 1.0    |
| 81    | 5 | 1.0    |
| 82    | 6 | 0.9987 |
| 88    | 5 | 1.0    |
| 91    | 6 | 0.9952 |
| 99    | 6 | 0.9907 |
| 100   | 5 | 1.0    |

実行時間を10倍にした場合。
良くなってはいるがあまり効いてない。

| seed  | 9 sec  | 90 sec  |
|-------|--------|---------|
| 3     | 0.9962 | 0.9992  |
| 4     | 0.9898 | 0.9939  |
| 13    | 0.9951 | 0.9978  |
| 14    | 1.0    | 1.0     |
| 19    | 0.9760 | 0.9866  |
| 22    | 0.9931 | 0.9938  |
| 24    | 0.9942 | 0.9956  |
| 31    | 0.9922 | 0.9957  |
| 33    | 0.9921 | 0.9980  |
| 37    | 0.9928 | 0.9976  |
| 43    | 0.9955 | 0.9979  |
| 50    | 0.9886 | 0.9977  |
| 53    | 0.9966 | 0.9987  |
| 57    | 0.9898 | 0.9937  |
| 58    | 0.9905 | 0.9937  |
| 82    | 0.9987 | 0.9968  |
| 91    | 0.9952 | 0.9984  |
| 99    | 0.9907 | 0.9972  |


`271521042ab7e8b8fe5b6b79dff4916ff8c6c1da` ちゃんとした焼き鈍し。
結局いつもの思考停止焼き鈍しになってしまったなあという気持ち。

| seed  | random | HC     | SA     |
|-------|--------|--------|--------|
| 3     | 0.9962 | 0.9988 | 0.9980 |
| 4     | 0.9898 | 0.9967 | 0.9888 |
| 13    | 0.9951 | 0.9918 | 0.9972 |
| 14    | 1.0    | 0.9886 | 1.0    |
| 19    | 0.9760 | 0.9813 | 1.0    |
| 22    | 0.9931 | 0.9965 | 0.9986 |
| 24    | 0.9942 | 0.9681 | 1.0    |
| 31    | 0.9922 | 0.9943 | 0.9918 |
| 33    | 0.9921 | 0.9960 | 1.0    |
| 37    | 0.9928 | 0.9960 | 1.0    |
| 43    | 0.9955 | 0.9919 | 0.9975 |
| 50    | 0.9886 | 1.0    | 0.9977 |
| 53    | 0.9966 | 0.9959 | 0.9946 |
| 57    | 0.9898 | 0.9937 | 0.9945 |
| 58    | 0.9905 | 0.9874 | 1.0    |
| 82    | 0.9987 | 0.9968 | 0.9974 |
| 91    | 0.9952 | 0.9973 | 1.0    |
| 99    | 0.9907 | 0.9926 | 0.9944 |

同様に制限時間を伸ばした場合。SAは速度の改善が十分有効のようだ。

| seed  | 1 sec  | 90 sec |
|-------|--------|--------|
| 3     | 0.9980 | 0.9992 |
| 4     | 0.9888 | 1.0    |
| 13    | 0.9972 | 1.0    |
| 14    | 1.0    | 1.0    |
| 19    | 1.0    | 1.0    |
| 22    | 0.9986 | 1.0    |
| 24    | 1.0    | 1.0    |
| 31    | 0.9918 | 0.9992 |
| 33    | 1.0    | 1.0    |
| 37    | 1.0    | 1.0    |
| 43    | 0.9975 | 1.0    |
| 50    | 0.9977 | 1.0    |
| 53    | 0.9946 | 0.9984 |
| 57    | 0.9945 | 1.0    |
| 58    | 1.0    | 1.0    |
| 82    | 0.9974 | 1.0    |
| 91    | 1.0    | 0.9986 |
| 99    | 0.9944 | 1.0    |

次に複数回試行した結果。ばらつくようなら初期解への依存が大きいなどが言えるだろうと思ったが、いまいちよく分からない結果。

| seed | A      | B      | C      | D      | E      | F      | G      | H      | I      | J      |
|------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| 3    | 0.998  | 0.9976 | 0.9972 | 0.9984 | 0.9982 | 0.998  | 0.9974 | 0.9968 | 0.9956 | 0.997  |
| 4    | 0.9958 | 0.9930 | 0.9972 | 0.9958 | 0.9953 | 0.9939 | 0.9925 | 0.9916 | 0.9925 | 0.9953 |
| 13   | 0.9989 | 0.9956 | 0.9967 | 0.9983 | 0.9978 | 1.0    | 0.9935 | 0.9972 | 0.9983 | 0.9945 |
| 14   | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    |
| 19   | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    |
| 22   | 0.9945 | 0.9890 | 0.9938 | 0.9951 | 0.9986 | 0.9855 | 1.0    | 0.9965 | 0.9938 | 0.9972 |
| 24   | 0.9971 | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 0.9956 | 1.0    | 1.0    | 0.9971 |
| 31   | 0.9964 | 0.9876 | 0.9943 | 0.9922 | 0.9940 | 0.9922 | 0.9901 | 0.9947 | 0.9943 | 0.9975 |
| 33   | 0.9970 | 1.0    | 0.9940 | 1.0    | 0.9931 | 0.9980 | 0.9970 | 1.0    | 1.0    | 1.0    |
| 37   | 1.0    | 1.0    | 0.9976 | 0.9960 | 1.0    | 0.9976 | 1.0    | 1.0    | 0.9984 | 0.9976 |
| 43   | 0.9963 | 0.9899 | 0.9919 | 0.9971 | 1.0    | 0.9963 | 0.9919 | 0.9971 | 0.9971 | 0.9903 |
| 50   | 0.9954 | 0.9965 | 1.0    | 1.0    | 1.0    | 0.9977 | 1.0    | 1.0    | 1.0    | 1.0    |
| 53   | 0.9984 | 0.9969 | 0.9976 | 0.9994 | 0.9918 | 0.9987 | 0.9979 | 0.9969 | 0.9966 | 0.9948 |
| 57   | 0.9952 | 0.9945 | 1.0    | 1.0    | 0.9945 | 0.9976 | 0.9968 | 0.9913 | 0.9913 | 0.9984 |
| 58   | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    | 1.0    |
| 82   | 0.9987 | 1.0    | 0.9962 | 1.0    | 0.9968 | 0.9962 | 1.0    | 1.0    | 0.9987 | 0.9962 |
| 91   | 0.9986 | 0.9992 | 0.9986 | 0.9984 | 0.9962 | 1.0    | 0.9981 | 0.9986 | 0.9931 | 0.9984 |
| 99   | 1.0    | 0.9981 | 1.0    | 0.9972 | 1.0    | 0.9981 | 1.0    | 1.0    | 0.9972 | 0.9916 |

`271521042ab7e8b8fe5b6b79dff4916ff8c6c1da` の提出結果。 903445.38 -> 910336.13 と改善はしているが順位は変わらず。
解法の自明さと点数の近さから見て、 [sdya](https://www.topcoder.com/members/sdya/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんも [shamal](https://www.topcoder.com/members/shamal/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんも解法は私とほぼ同じに思える。

1位取ったつもりで投げたのに全然だめで冷えた。
ほぼなかった満点の数が半分くらいになったし「単独1位だろうから順位表を見れば「どうせあいつのことだから思考停止焼き鈍し/ビームサーチでラッキーパンチでしょ」というメタができてしまう気がする提出控えた方がいいかなあ」「でも暫定1位にして気持ちよくなりたい」などと考えていたのはなんだったのか、悲しすぎる。

[kurenai3110](https://www.topcoder.com/members/kurenai3110/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんや [yowa](https://www.topcoder.com/members/yowa/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんがまだ下の方なのはなぜだろう。そろそろ彼らが1,2位あたりに座る頃の印象がある。
[wleite](https://www.topcoder.com/members/wleite/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんや [hakomo](https://www.topcoder.com/members/hakomo/details/?track=DATA_SCIENCE&subTrack=MARATHON_MATCH) さんあたりの初動が遅いのはこれはいつもそうだった気がする。

| Handle     | Score     | Rank      | Last Submission Time     | Language  | Example Tests  | Submissions    | Current Rating | Predicted Rating    | Rating Delta |
|------------|-----------|-----------|--------------------------|-----------|----------------|----------------|----------------|---------------------|--------------|
| sdya       | 913445.38 | 1         | 04.19.2018 17:11:35      | C++       | 1              | 1              | 2163           | 2319.33             | 156.33       |
| shamal     | 910798.32 | 2         | 04.20.2018 09:10:22      | C++       | 7              | 4              | 1627           | 1897.04             | 270.04       |
| kimiyuki   | 910336.13 | 3         | 04.20.2018 10:57:17      | C++       | 19             | 6              | 1745           | 1982.81             | 237.81       |
| JacoCronje | 902647.06 | 4         | 04.20.2018 08:59:33      | C++       | 3              | 3              | 2231           | 2287.71             | 56.71        |
| CatalinT   | 894327.73 | 5         | 04.20.2018 04:06:54      | C++       | 4              | 4              | 2504           | 2512.00             | 8.00         |

次の手はとりあえず高速化と焼き鈍し変数調整だろうか。しかし日程を見ても解法を見てもそういうのはまだ早い気がする。どうしたものか。
