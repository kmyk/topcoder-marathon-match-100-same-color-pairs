# TopCoder Marathon Match 100: SameColorPairs

<https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=17143&pm=14889>

## results

I won 1st place. My solution got the perfect score 1.000 for all of 1999 test cases and failed in only one test case.

| Rank  | Handle        | Provisional Rank      | Provisional Score     | Final Score   | Language      |
|-------|---------------|-----------------------|-----------------------|---------------|---------------|
| 1     | kimiyuki      | 1                     | 813,131.67            | 791,337.19    | C++           |
| 2     | eldidou       | 1                     | 813,131.67            | 791,302.49    | C++           |
| 3     | imazato       | 1                     | 813,131.67            | 791,285.59    | C++           |
| 4     | wleite        | 1                     | 813,131.67            | 791,273.13    | Java          |
| 5     | hakomo        | 6                     | 812,366.55            | 791,163.70    | C++           |

<https://community.topcoder.com/longcontest/stats/?module=ViewOverview&rd=17143>

## algorithm

The main algorithm is: split the board into chunks and do simulated annealing chunkwise.

Doing simulated annealing is the typical one.
Splitting boards is effective since cases with moderately small areas are easy.

![a40dfcc54f8f2cdd9155d0fc5ceb02315e76d1b2.1](https://user-images.githubusercontent.com/2203128/39192521-4eabe0ac-4814-11e8-83b9-be3d79b93b13.gif)

![4165](https://github.com/kmyk/topcoder-marathon-match-100-same-color-pairs/raw/master/image/4165.gif)

[Re: Post your approach, TopCoder Forums](https://apps.topcoder.com/forums/?module=Thread&threadID=916946&start=0&mc=13#2263153)
