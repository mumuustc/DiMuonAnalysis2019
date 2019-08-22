#ifndef RunInfo_PbPb_Run5TeV_eventUtils_H_
#define RunInfo_PbPb_Run5TeV_eventUtils_H_


#include <iostream>
#include "Y2015/eventUtils.h"
#include "Y2018/eventUtils.h"


namespace PbPb {
  namespace R5TeV {
    // Centrality information
    double findNcoll(int hiBin)
    {
      const int nbins = 200;
      const double Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
      return Ncoll[hiBin];
    };
    //
    double findNcollAverage(int hiBinLow, int hiBinHigh)
    {
      // if the bin corresponds to a bin we already know, give the official number from https://twiki.cern.ch/twiki/bin/view/CMS/GlauberTables
      if (hiBinLow==0&&hiBinHigh==20) return 1626;
      if (hiBinLow==20&&hiBinHigh==40) return 1005;
      if (hiBinLow==40&&hiBinHigh==60) return 606.2;
      if (hiBinLow==60&&hiBinHigh==80) return 348.5;
      if (hiBinLow==80&&hiBinHigh==100) return 186.2;
      if (hiBinLow==100&&hiBinHigh==200) return 30.76;
      if (hiBinLow==20&&hiBinHigh==60) return 805.4;
      if (hiBinLow==60&&hiBinHigh==200) return 98.36;
      if (hiBinLow==0&&hiBinHigh==10) return 1819;
      if (hiBinLow==10&&hiBinHigh==20) return 1432;
      if (hiBinLow==20&&hiBinHigh==30) return 1127;
      if (hiBinLow==30&&hiBinHigh==40) return 882;
      if (hiBinLow==40&&hiBinHigh==50) return 685.9;
      if (hiBinLow==50&&hiBinHigh==60) return 526.5;
      if (hiBinLow==60&&hiBinHigh==70) return 399.4;
      if (hiBinLow==70&&hiBinHigh==80) return 297.6;
      if (hiBinLow==80&&hiBinHigh==90) return 217.2;
      if (hiBinLow==90&&hiBinHigh==100) return 155.5;
      if (hiBinLow==100&&hiBinHigh==120) return 90.72;
      if (hiBinLow==120&&hiBinHigh==140) return 40.1;
      if (hiBinLow==140&&hiBinHigh==200) return 7.663;
      if (hiBinLow==0&&hiBinHigh==40) return 1315;
      if (hiBinLow==40&&hiBinHigh==80) return 477.3;
      if (hiBinLow==80&&hiBinHigh==200) return 56.67;
      if (hiBinLow==0&&hiBinHigh==200) return 392.5;
      // else... compute the average ncoll ourselves
      double w=0;
      const int nbins = 200;
      const double Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
      for(int i=hiBinLow; i<hiBinHigh; i++)  w += Ncoll[i]/(hiBinHigh-hiBinLow);
      return w;
    };
    //
    double findNpartSyst_low(int hiBinLow, int hiBinHigh)
    {
      // if the bin corresponds to a bin we already know, give the official number from https://twiki.cern.ch/twiki/bin/view/CMS/GlauberTables
      if (hiBinLow==0&&hiBinHigh==20) return 2.6;
      if (hiBinLow==20&&hiBinHigh==40) return 3.8;
      if (hiBinLow==40&&hiBinHigh==60) return 4.1;
      if (hiBinLow==60&&hiBinHigh==80) return 4.0;
      if (hiBinLow==80&&hiBinHigh==100) return 3.7;
      if (hiBinLow==100&&hiBinHigh==200) return 1.0;
      if (hiBinLow==20&&hiBinHigh==60) return 3.9;
      if (hiBinLow==60&&hiBinHigh==200) return 1.2;
      if (hiBinLow==0&&hiBinHigh==10) return 2.0;
      if (hiBinLow==10&&hiBinHigh==20) return 3.2;
      if (hiBinLow==20&&hiBinHigh==30) return 3.7;
      if (hiBinLow==30&&hiBinHigh==40) return 3.9;
      if (hiBinLow==40&&hiBinHigh==50) return 4.1;
      if (hiBinLow==50&&hiBinHigh==60) return 4.0;
      if (hiBinLow==60&&hiBinHigh==70) return 4.0;
      if (hiBinLow==70&&hiBinHigh==80) return 4.0;
      if (hiBinLow==80&&hiBinHigh==90) return 3.8;
      if (hiBinLow==90&&hiBinHigh==100) return 3.6;
      if (hiBinLow==100&&hiBinHigh==120) return 3.1;
      if (hiBinLow==120&&hiBinHigh==140) return 2.4;
      if (hiBinLow==140&&hiBinHigh==200) return 0.6;
      if (hiBinLow==0&&hiBinHigh==40) return 3.1;
      if (hiBinLow==40&&hiBinHigh==80) return 4.0;
      if (hiBinLow==80&&hiBinHigh==200) return 1.1;
      if (hiBinLow==0&&hiBinHigh==200) return 2.6;
      std::cout << "[ERROR}: Official Npart low unc not found for range [" << hiBinLow << "," << hiBinHigh <<   "]" << std::endl;
      return -5;
    };
  
    double findNpartSyst_high(int hiBinLow, int hiBinHigh)
    {
      // if the bin corresponds to a bin we already know, give the official number from https://twiki.cern.ch/twiki/bin/view/CMS/GlauberTables
      if (hiBinLow==0&&hiBinHigh==20) return 2.4;
      if (hiBinLow==20&&hiBinHigh==40) return 3.6;
      if (hiBinLow==40&&hiBinHigh==60) return 4.0;
      if (hiBinLow==60&&hiBinHigh==80) return 4.0;
      if (hiBinLow==80&&hiBinHigh==100) return 3.7;
      if (hiBinLow==100&&hiBinHigh==200) return 1.8;
      if (hiBinLow==20&&hiBinHigh==60) return 3.7;
      if (hiBinLow==60&&hiBinHigh==200) return 2.4;
      if (hiBinLow==0&&hiBinHigh==10) return 1.8;
      if (hiBinLow==10&&hiBinHigh==20) return 3.0;
      if (hiBinLow==20&&hiBinHigh==30) return 3.5;
      if (hiBinLow==30&&hiBinHigh==40) return 3.8;
      if (hiBinLow==40&&hiBinHigh==50) return 3.9;
      if (hiBinLow==50&&hiBinHigh==60) return 4.0;
      if (hiBinLow==60&&hiBinHigh==70) return 4.0;
      if (hiBinLow==70&&hiBinHigh==80) return 4.0;
      if (hiBinLow==80&&hiBinHigh==90) return 3.8;
      if (hiBinLow==90&&hiBinHigh==100) return 3.7;
      if (hiBinLow==100&&hiBinHigh==120) return 3.2;
      if (hiBinLow==120&&hiBinHigh==140) return 2.6;
      if (hiBinLow==140&&hiBinHigh==200) return 1.0;
      if (hiBinLow==0&&hiBinHigh==40) return 2.9;
      if (hiBinLow==40&&hiBinHigh==80) return 4.0;
      if (hiBinLow==80&&hiBinHigh==200) return 2.1;
      if (hiBinLow==0&&hiBinHigh==200) return 2.6;
      std::cout << "[ERROR}: Official Npart high unc not found for range [" << hiBinLow << "," << hiBinHigh << "]" << std::endl;
      return -5;
    };
    //
    float findNpart(int hiBin)
    {
      const int nbins = 200;
      const float Npart[nbins] = {401.99, 398.783, 396.936, 392.71, 387.901, 383.593, 377.914, 374.546, 367.507, 361.252, 356.05, 352.43, 345.701, 341.584, 335.148, 330.581, 325.135, 320.777, 315.074, 310.679, 306.687, 301.189, 296.769, 291.795, 287.516, 283.163, 277.818, 274.293, 269.29, 265.911, 260.574, 256.586, 252.732, 249.194, 245.011, 241.292, 236.715, 232.55, 229.322, 225.328, 221.263, 218.604, 214.728, 210.554, 206.878, 203.924, 200.84, 196.572, 193.288, 189.969, 186.894, 183.232, 180.24, 177.36, 174.008, 171.222, 168.296, 165.319, 162.013, 158.495, 156.05, 154.218, 150.559, 148.455, 145.471, 142.496, 139.715, 137.395, 134.469, 131.926, 129.817, 127.045, 124.467, 122.427, 119.698, 117.607, 114.543, 112.662, 110.696, 108.294, 105.777, 103.544, 101.736, 99.943, 97.4951, 95.4291, 93.2148, 91.2133, 89.5108, 87.2103, 85.7498, 83.5134, 81.9687, 79.7456, 78.1684, 76.4873, 74.7635, 72.761, 71.0948, 69.6102, 67.7806, 66.2215, 64.5813, 63.0269, 61.4325, 59.8065, 58.2423, 57.2432, 55.8296, 54.2171, 52.8809, 51.3254, 49.9902, 48.6927, 47.5565, 46.136, 44.8382, 43.6345, 42.3964, 41.4211, 39.9681, 39.178, 37.9341, 36.9268, 35.5626, 34.5382, 33.6912, 32.8156, 31.6695, 30.6552, 29.7015, 28.8655, 27.9609, 27.0857, 26.105, 25.3163, 24.4872, 23.6394, 23.0484, 22.2774, 21.4877, 20.5556, 19.9736, 19.3296, 18.5628, 17.916, 17.2928, 16.6546, 16.1131, 15.4013, 14.8264, 14.3973, 13.7262, 13.2853, 12.8253, 12.2874, 11.7558, 11.2723, 10.8829, 10.4652, 9.96477, 9.6368, 9.09316, 8.84175, 8.48084, 8.05694, 7.64559, 7.29709, 7.07981, 6.70294, 6.45736, 6.10284, 5.91788, 5.5441, 5.33311, 5.06641, 4.96415, 4.6286, 4.38214, 4.2076, 4.01099, 3.81054, 3.63854, 3.43403, 3.23244, 3.08666, 2.86953, 2.74334, 2.62787, 2.48354, 2.38115, 2.26822, 2.23137, 2.1665, 2.14264, 2.10636, 2.07358, 2.05422, 2.04126, 2.00954};
      return Npart[hiBin];
    };
    //
    float findNpartAverage(int hiBinLow, int hiBinHigh)
    {
      // if the bin corresponds to a bin we already know, give the official number from https://twiki.cern.ch/twiki/bin/view/CMS/GlauberTables
      if (hiBinLow==0&&hiBinHigh==20) return 358.8;
      if (hiBinLow==20&&hiBinHigh==40) return 264.2;
      if (hiBinLow==40&&hiBinHigh==60) return 189.2;
      if (hiBinLow==60&&hiBinHigh==80) return 131.4;
      if (hiBinLow==80&&hiBinHigh==100) return 86.95;
      if (hiBinLow==100&&hiBinHigh==200) return 21.87;
      if (hiBinLow==20&&hiBinHigh==60) return 226.7;
      if (hiBinLow==60&&hiBinHigh==200) return 46.81;
      if (hiBinLow==0&&hiBinHigh==10) return 384.3;
      if (hiBinLow==10&&hiBinHigh==20) return 333.3;
      if (hiBinLow==20&&hiBinHigh==30) return 285.4;
      if (hiBinLow==30&&hiBinHigh==40) return 242.9;
      if (hiBinLow==40&&hiBinHigh==50) return 205.7;
      if (hiBinLow==50&&hiBinHigh==60) return 172.7;
      if (hiBinLow==60&&hiBinHigh==70) return 144.1;
      if (hiBinLow==70&&hiBinHigh==80) return 118.7;
      if (hiBinLow==80&&hiBinHigh==90) return 96.51;
      if (hiBinLow==90&&hiBinHigh==100) return 77.39;
      if (hiBinLow==100&&hiBinHigh==120) return 53.86;
      if (hiBinLow==120&&hiBinHigh==140) return 30.57;
      if (hiBinLow==140&&hiBinHigh==200) return 8.297;
      if (hiBinLow==0&&hiBinHigh==40) return 311.5;
      if (hiBinLow==40&&hiBinHigh==80) return 160.3;
      if (hiBinLow==80&&hiBinHigh==200) return 32.71;
      if (hiBinLow==0&&hiBinHigh==200) return 114.0;    
      // else... compute the average npart ourselves
      float w=0;
      const int nbins = 200;
      const float Npart[nbins] = {401.99, 398.783, 396.936, 392.71, 387.901, 383.593, 377.914, 374.546, 367.507, 361.252, 356.05, 352.43, 345.701, 341.584, 335.148, 330.581, 325.135, 320.777, 315.074, 310.679, 306.687, 301.189, 296.769, 291.795, 287.516, 283.163, 277.818, 274.293, 269.29, 265.911, 260.574, 256.586, 252.732, 249.194, 245.011, 241.292, 236.715, 232.55, 229.322, 225.328, 221.263, 218.604, 214.728, 210.554, 206.878, 203.924, 200.84, 196.572, 193.288, 189.969, 186.894, 183.232, 180.24, 177.36, 174.008, 171.222, 168.296, 165.319, 162.013, 158.495, 156.05, 154.218, 150.559, 148.455, 145.471, 142.496, 139.715, 137.395, 134.469, 131.926, 129.817, 127.045, 124.467, 122.427, 119.698, 117.607, 114.543, 112.662, 110.696, 108.294, 105.777, 103.544, 101.736, 99.943, 97.4951, 95.4291, 93.2148, 91.2133, 89.5108, 87.2103, 85.7498, 83.5134, 81.9687, 79.7456, 78.1684, 76.4873, 74.7635, 72.761, 71.0948, 69.6102, 67.7806, 66.2215, 64.5813, 63.0269, 61.4325, 59.8065, 58.2423, 57.2432, 55.8296, 54.2171, 52.8809, 51.3254, 49.9902, 48.6927, 47.5565, 46.136, 44.8382, 43.6345, 42.3964, 41.4211, 39.9681, 39.178, 37.9341, 36.9268, 35.5626, 34.5382, 33.6912, 32.8156, 31.6695, 30.6552, 29.7015, 28.8655, 27.9609, 27.0857, 26.105, 25.3163, 24.4872, 23.6394, 23.0484, 22.2774, 21.4877, 20.5556, 19.9736, 19.3296, 18.5628, 17.916, 17.2928, 16.6546, 16.1131, 15.4013, 14.8264, 14.3973, 13.7262, 13.2853, 12.8253, 12.2874, 11.7558, 11.2723, 10.8829, 10.4652, 9.96477, 9.6368, 9.09316, 8.84175, 8.48084, 8.05694, 7.64559, 7.29709, 7.07981, 6.70294, 6.45736, 6.10284, 5.91788, 5.5441, 5.33311, 5.06641, 4.96415, 4.6286, 4.38214, 4.2076, 4.01099, 3.81054, 3.63854, 3.43403, 3.23244, 3.08666, 2.86953, 2.74334, 2.62787, 2.48354, 2.38115, 2.26822, 2.23137, 2.1665, 2.14264, 2.10636, 2.07358, 2.05422, 2.04126, 2.00954};
      for(int i=hiBinLow; i<hiBinHigh; i++)  w += Npart[i]/(hiBinHigh-hiBinLow);
      return w;
    };
    //
    float findTaaAverage(int hiBinLow, int hiBinHigh)
    {
      // if the bin corresponds to a bin we already know, give the official number from https://twiki.cern.ch/twiki/bin/view/CMS/GlauberTables
      if (hiBinLow==0&&hiBinHigh==20) return 23.22;
      if (hiBinLow==20&&hiBinHigh==40) return 14.35;
      if (hiBinLow==40&&hiBinHigh==60) return 8.660;
      if (hiBinLow==60&&hiBinHigh==80) return 4.978;
      if (hiBinLow==80&&hiBinHigh==100) return 2.660;
      if (hiBinLow==100&&hiBinHigh==200) return 0.4395;
      if (hiBinLow==20&&hiBinHigh==60) return 11.51;
      if (hiBinLow==60&&hiBinHigh==200) return 1.405;
      if (hiBinLow==0&&hiBinHigh==10) return 25.98;
      if (hiBinLow==10&&hiBinHigh==20) return 20.46;
      if (hiBinLow==20&&hiBinHigh==30) return 16.11;
      if (hiBinLow==30&&hiBinHigh==40) return 12.60;
      if (hiBinLow==40&&hiBinHigh==50) return 9.799;
      if (hiBinLow==50&&hiBinHigh==60) return 7.522;
      if (hiBinLow==60&&hiBinHigh==70) return 5.706;
      if (hiBinLow==70&&hiBinHigh==80) return 4.251;
      if (hiBinLow==80&&hiBinHigh==90) return 3.103;
      if (hiBinLow==90&&hiBinHigh==100) return 2.217;
      if (hiBinLow==100&&hiBinHigh==120) return 1.296;
      if (hiBinLow==120&&hiBinHigh==140) return 0.5729;
      if (hiBinLow==140&&hiBinHigh==200) return 0.1095;
      if (hiBinLow==0&&hiBinHigh==40) return 18.79;
      if (hiBinLow==40&&hiBinHigh==80) return 6.819;
      if (hiBinLow==80&&hiBinHigh==200) return 0.8096;
      if (hiBinLow==0&&hiBinHigh==200) return 5.607;
      std::cout << "[ERROR]: Official Taa number not found for range [" << hiBinLow << "," << hiBinHigh << "]" << std::endl;
      return -1;
    };
    //
    float findTaaAverage_err_low(int hiBinLow, int hiBinHigh)
    {
      // if the bin corresponds to a bin we already know, give the official number from https://twiki.cern.ch/twiki/bin/view/CMS/GlauberTables
      if (hiBinLow==0&&hiBinHigh==20) return 0.689;
      if (hiBinLow==20&&hiBinHigh==40) return 0.455;
      if (hiBinLow==40&&hiBinHigh==60) return 0.332;
      if (hiBinLow==60&&hiBinHigh==80) return 0.242;
      if (hiBinLow==80&&hiBinHigh==100) return 0.176;
      if (hiBinLow==100&&hiBinHigh==200) return 0.032;
      if (hiBinLow==20&&hiBinHigh==60) return 0.388;
      if (hiBinLow==60&&hiBinHigh==200) return 0.061;
      if (hiBinLow==0&&hiBinHigh==10) return 0.766;
      if (hiBinLow==10&&hiBinHigh==20) return 0.605;
      if (hiBinLow==20&&hiBinHigh==30) return 0.496;
      if (hiBinLow==30&&hiBinHigh==40) return 0.427;
      if (hiBinLow==40&&hiBinHigh==50) return 0.374;
      if (hiBinLow==50&&hiBinHigh==60) return 0.316;
      if (hiBinLow==60&&hiBinHigh==70) return 0.270;
      if (hiBinLow==70&&hiBinHigh==80) return 0.236;
      if (hiBinLow==80&&hiBinHigh==90) return 0.190;
      if (hiBinLow==90&&hiBinHigh==100) return 0.157;
      if (hiBinLow==100&&hiBinHigh==120) return 0.115;
      if (hiBinLow==120&&hiBinHigh==140) return 0.064;
      if (hiBinLow==140&&hiBinHigh==200) return 0.011;
      if (hiBinLow==0&&hiBinHigh==40) return 0.563;
      if (hiBinLow==40&&hiBinHigh==80) return 0.283;
      if (hiBinLow==80&&hiBinHigh==200) return 0.046;
      if (hiBinLow==0&&hiBinHigh==200) return 0.191;
      std::cout << "[ERROR]: Official Taa lower uncertainty not found for range [" << hiBinLow << "," << hiBinHigh << "]" << std::endl;
      return -1;
    };
    //
    float findTaaAverage_err_high(int hiBinLow, int hiBinHigh)
    {
      // if the bin corresponds to a bin we already know, give the official number from https://twiki.cern.ch/twiki/bin/view/CMS/GlauberTables
      if (hiBinLow==0&&hiBinHigh==20) return 0.431;
      if (hiBinLow==20&&hiBinHigh==40) return 0.329;
      if (hiBinLow==40&&hiBinHigh==60) return 0.288;
      if (hiBinLow==60&&hiBinHigh==80) return 0.236;
      if (hiBinLow==80&&hiBinHigh==100) return 0.180;
      if (hiBinLow==100&&hiBinHigh==200) return 0.049;
      if (hiBinLow==20&&hiBinHigh==60) return 0.304;
      if (hiBinLow==60&&hiBinHigh==200) return 0.094;
      if (hiBinLow==0&&hiBinHigh==10) return 0.469;
      if (hiBinLow==10&&hiBinHigh==20) return 0.382;
      if (hiBinLow==20&&hiBinHigh==30) return 0.352;
      if (hiBinLow==30&&hiBinHigh==40) return 0.321;
      if (hiBinLow==40&&hiBinHigh==50) return 0.311;
      if (hiBinLow==50&&hiBinHigh==60) return 0.294;
      if (hiBinLow==60&&hiBinHigh==70) return 0.265;
      if (hiBinLow==70&&hiBinHigh==80) return 0.230;
      if (hiBinLow==80&&hiBinHigh==90) return 0.192;
      if (hiBinLow==90&&hiBinHigh==100) return 0.162;
      if (hiBinLow==100&&hiBinHigh==120) return 0.120;
      if (hiBinLow==120&&hiBinHigh==140) return 0.071;
      if (hiBinLow==140&&hiBinHigh==200) return 0.018;
      if (hiBinLow==0&&hiBinHigh==40) return 0.366;
      if (hiBinLow==40&&hiBinHigh==80) return 0.260;
      if (hiBinLow==80&&hiBinHigh==200) return 0.071;
      if (hiBinLow==0&&hiBinHigh==200) return 0.155;
      std::cout << "[ERROR]: Official Taa lower uncertainty not found for range [" << hiBinLow << "," << hiBinHigh << "]" << std::endl;
      return -1;
    };
  };
};
  

#endif /* RunInfo_PbPb_Run5TeV_eventUtils_H_ */
