import numpy as np
import gpxpy
import gpxpy.gpx
import math

#設定値
MPLAT = 110940.5844 #lat1度あたりの距離(m)
MPLON = 91287.7885 #lon1度あたりの距離(m)
INTERVAL_LENGTH = 250 #補完後のpoint間距離(m)

#global変数
V = 0 #マップの垂直方向の長さ(m)(10mずつ余裕をとる)
H = 0 #マップの水平方向の長さ(m)(10mずつ余裕をとる)
ORIGIN = [0,0]#マップの左上の座標(度)
TERMINAL = [0,0] #マップの右下の座標(度)

#GPSファイルを開きlat, lonに値を格納
def openGPX(gpxfile):
    lat, lon = [], [] #GPXファイルの点の緯度経度情報(度)
    gpx_file = open(gpxfile, 'r')
    gpx = gpxpy.parse(gpx_file)
    for track in gpx.tracks:
        for segment in track.segments:
            for point in segment.points:
                lat.append(point.latitude)
                lon.append(point.longitude)
    return [lat,lon]

#point間を補完する
def complement(point):
  lat_complemented, lon_complemented = [], [] #補完されたlat,lonリスト(度)
  lat = point[0]
  lon = point[1]
  for i in range(0,len(lat)-1):
    #point間の距離を測る
    distance = (((lat[i+1] - lat[i]) * MPLAT)**2 + ((lon[i+1] - lon[i])*MPLON)**2)**0.5
    #point間の分割数を決める
    section = (int)(distance//INTERVAL_LENGTH)
    if section == 0:
      section = 1
    for sec in range(0,section):
      lat_complemented.append(lat[i] + (lat[i+1] - lat[i])/section * sec)
      lon_complemented.append(lon[i] + (lon[i+1] - lon[i])/section * sec)
  lat_complemented.append(lat[-1])
  lon_complemented.append(lon[-1])
  return [lat_complemented, lon_complemented]

#緯度経度情報を縦横メートル情報に置き換える
def degToMeter(point_complemented):
  lat2, lon2 = [], []#マップ上のlat,lonリスト(m)
  lat_complemented = point_complemented[0]
  lon_complemented = point_complemented[1]
  global V,H,ORIGIN,TERMINAL
  V = ((max(lat_complemented)-min(lat_complemented))*MPLAT)+INTERVAL_LENGTH*2
  H = ((max(lon_complemented)-min(lon_complemented))*MPLON)+INTERVAL_LENGTH*2
  ORIGIN = [max(lat_complemented) + INTERVAL_LENGTH/MPLAT, min(lon_complemented) - INTERVAL_LENGTH/MPLON]
  TERMINAL = [ORIGIN[0] - V/MPLAT, ORIGIN[1] + H/MPLON]
  for i in range(0, len(lat_complemented)):
    lat2.append((ORIGIN[0] - lat_complemented[i]) * MPLAT)
    lon2.append((lon_complemented[i] - ORIGIN[1]) * MPLON)
  return [lat2,lon2]

#判定マップの作成(使わない)
def makeMap(point2):
  lat2 = point2[0]
  lon2 = point2[1]
  judgeMap = np.array([[0]*H]*V)
  #経路ポイントごとにOUT_RANGE*2(m)四方は経路内とする
  for i in range(0, len(lat2)):
    for j in range(-OUT_RANGE, OUT_RANGE):
      for k in range(-OUT_RANGE, OUT_RANGE):
        judgeMap[lat2[i]+j][lon2[i]+k] = 1
  return judgeMap

#xを小数点2桁にする
def cut(x):
    return math.floor(x * 10 ** 2) / (10 ** 2)

#txtファイルの作成
def makeTxtFile(point2):
  #規定値
  MPLATstr = str(MPLAT)
  MPLONstr = str(MPLON)
  INTERVAL_LENGTHstr = str(INTERVAL_LENGTH)
  #global変数
  ORIGINstr = []
  TERMINALstr = []
  for i in range(len(ORIGIN)):
    ORIGINstr.append(str(ORIGIN[i]))
  for i in range(len(TERMINAL)):
    TERMINALstr.append(str(TERMINAL[i]))
  Vstr = str(cut(V))
  Hstr = str(cut(H))
  #point2
  point2_str = [[],[]]
  for i in range(len(point2)):
    for j in range(len(point2[i])):
      point2_str[i].append(str(cut(point2[i][j])))
  
  with open('output.txt', 'w') as f:
      f.writelines(MPLATstr)
      f.writelines('\n')
      f.writelines(MPLONstr)
      f.writelines('\n')
      f.writelines(INTERVAL_LENGTHstr)
      f.writelines('\n')
      f.writelines(ORIGINstr[0])
      f.writelines('\n')
      f.writelines(ORIGINstr[1])
      f.writelines('\n')
      f.writelines(TERMINALstr[0])
      f.writelines('\n')
      f.writelines(TERMINALstr[1])
      f.writelines('\n')
      f.writelines(Hstr)
      f.writelines('\n')
      f.writelines(Vstr)
      f.writelines('\n')
      for i in range(len(point2)):
        for j in range(len(point2[i])):
          f.writelines(point2_str[i][j])
          f.writelines(',')
        f.writelines('\n')
  f.close()

#実行ファイル
def main(gpxfile):
    point = openGPX(gpxfile)
    print("opened")
    point_complemented = complement(point)
    print("complemented")
    point2 = degToMeter(point_complemented)
    print("meter")
    makeTxtFile(point2)
    print("done")

if __name__ == '__main__':
    main('koganei.gpx')
