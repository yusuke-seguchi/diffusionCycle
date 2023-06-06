# diffusionCycle
結露サイクル試験を想定した環境下で、1dの拡散方程式を解くプログラム。
境界条件は、ゼロノイマン条件。
温度tempは全体で、一定と仮定している。
左端から10gridまでを外気として、一定の湿度を設定するようにした。
ある時間間隔stepSwitchで、環境（温度tempと湿度humid）が切り替わるようにした。
