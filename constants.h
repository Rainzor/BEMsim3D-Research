#pragma once
#include <cmath>
#include <complex>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <vector>

using namespace Eigen;
using namespace std;

typedef complex<double> dcomp;
typedef complex<float> fcomp;

static MatrixXd quadrature_points {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.5773502691896257, 0.5773502691896257, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.7745966692414834, 0, 0.7745966692414834, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.8611363115940526, -0.3399810435848563, 0.3399810435848563, 0.8611363115940526, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9061798459386640, -0.5384693101056831, 0, 0.5384693101056831, 0.9061798459386640, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9324695142031521, -0.6612093864662645, -0.2386191860831969, 0.2386191860831969, 0.6612093864662645, 0.9324695142031521, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9491079123427585, -0.7415311855993945, -0.4058451513773972, 0, 0.4058451513773972, 0.7415311855993945, 0.9491079123427585, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9602898564975363, -0.7966664774136267, -0.5255324099163290, -0.1834346424956498, 0.1834346424956498, 0.5255324099163290, 0.7966664774136267, 0.9602898564975363, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9681602395076261, -0.8360311073266358, -0.6133714327005904, -0.3242534234038089, 0, 0.3242534234038089, 0.6133714327005904, 0.8360311073266358, 0.9681602395076261, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9739065285171717, -0.8650633666889845, -0.6794095682990244, -0.4333953941292472, -0.1488743389816312, 0.1488743389816312, 0.4333953941292472, 0.6794095682990244, 0.8650633666889845, 0.9739065285171717, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9782286581460570, -0.8870625997680953, -0.7301520055740494, -0.5190961292068118, -0.2695431559523450, 0, 0.2695431559523450, 0.5190961292068118, 0.7301520055740494, 0.8870625997680953, 0.9782286581460570, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9815606342467192, -0.9041172563704749, -0.7699026741943047, -0.5873179542866175, -0.3678314989981802, -0.1252334085114689, 0.1252334085114689, 0.3678314989981802, 0.5873179542866175, 0.7699026741943047, 0.9041172563704749, 0.9815606342467192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9841830547185881, -0.9175983992229779, -0.8015780907333099, -0.6423493394403402, -0.4484927510364469, -0.2304583159551348, 0, 0.2304583159551348, 0.4484927510364469, 0.6423493394403402, 0.8015780907333099, 0.9175983992229779, 0.9841830547185881, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9862838086968123, -0.9284348836635735, -0.8272013150697650, -0.6872929048116855, -0.5152486363581541, -0.3191123689278897, -0.1080549487073437, 0.1080549487073437, 0.3191123689278897, 0.5152486363581541, 0.6872929048116855, 0.8272013150697650, 0.9284348836635735, 0.9862838086968123, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9879925180204854, -0.9372733924007060, -0.8482065834104272, -0.7244177313601701, -0.5709721726085388, -0.3941513470775634, -0.2011940939974345, 0, 0.2011940939974345, 0.3941513470775634, 0.5709721726085388, 0.7244177313601701, 0.8482065834104272, 0.9372733924007060, 0.9879925180204854, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9894009349916499, -0.9445750230732326, -0.8656312023878318, -0.7554044083550030, -0.6178762444026438, -0.4580167776572274, -0.2816035507792589, -0.0950125098376374, 0.0950125098376374, 0.2816035507792589, 0.4580167776572274, 0.6178762444026438, 0.7554044083550030, 0.8656312023878318, 0.9445750230732326, 0.9894009349916499, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9905754753144174, -0.9506755217687678, -0.8802391537269859, -0.7815140038968014, -0.6576711592166907, -0.5126905370864769, -0.3512317634538763, -0.1784841814958479, 0, 0.1784841814958479, 0.3512317634538763, 0.5126905370864769, 0.6576711592166907, 0.7815140038968014, 0.8802391537269859, 0.9506755217687678, 0.9905754753144174, 0, 0, 0, 0, 0, 0, 0, 0},
    {-0.9915651684209309, -0.9558239495713977, -0.8926024664975557, -0.8037049589725231, -0.6916870430603532, -0.5597708310739475, -0.4117511614628426, -0.2518862256915055, -0.0847750130417353, 0.0847750130417353, 0.2518862256915055, 0.4117511614628426, 0.5597708310739475, 0.6916870430603532, 0.8037049589725231, 0.8926024664975557, 0.9558239495713977, 0.9915651684209309, 0, 0, 0, 0, 0, 0, 0},
    {-0.9924068438435844, -0.9602081521348300, -0.9031559036148179, -0.8227146565371428, -0.7209661773352294, -0.6005453046616810, -0.4645707413759609, -0.3165640999636298, -0.1603586456402254, 0, 0.1603586456402254, 0.3165640999636298, 0.4645707413759609, 0.6005453046616810, 0.7209661773352294, 0.8227146565371428, 0.9031559036148179, 0.9602081521348300, 0.9924068438435844, 0, 0, 0, 0, 0, 0},
    {-0.9931285991850949, -0.9639719272779138, -0.9122344282513259, -0.8391169718222188, -0.7463319064601508, -0.6360536807265150, -0.5108670019508271, -0.3737060887154195, -0.2277858511416451, -0.0765265211334973, 0.0765265211334973, 0.2277858511416451, 0.3737060887154195, 0.5108670019508271, 0.6360536807265150, 0.7463319064601508, 0.8391169718222188, 0.9122344282513259, 0.9639719272779138, 0.9931285991850949, 0, 0, 0, 0, 0},
    {-0.9937521706203895, -0.9672268385663063, -0.9200993341504008, -0.8533633645833173, -0.7684399634756779, -0.6671388041974123, -0.5516188358872198, -0.4243421202074388, -0.2880213168024011, -0.1455618541608951, 0, 0.1455618541608951, 0.2880213168024011, 0.4243421202074388, 0.5516188358872198, 0.6671388041974123, 0.7684399634756779, 0.8533633645833173, 0.9200993341504008, 0.9672268385663063, 0.9937521706203895, 0, 0, 0, 0},
    {-0.9942945854823992, -0.9700604978354287, -0.9269567721871740, -0.8658125777203002, -0.7878168059792081, -0.6944872631866827, -0.5876404035069116, -0.4693558379867570, -0.3419358208920842, -0.2078604266882213, -0.0697392733197222, 0.0697392733197222, 0.2078604266882213, 0.3419358208920842, 0.4693558379867570, 0.5876404035069116, 0.6944872631866827, 0.7878168059792081, 0.8658125777203002, 0.9269567721871740, 0.9700604978354287, 0.9942945854823992, 0, 0, 0},
    {-0.9947693349975522, -0.9725424712181152, -0.9329710868260161, -0.8767523582704416, -0.8048884016188399, -0.7186613631319502, -0.6196098757636461, -0.5095014778460075, -0.3903010380302908, -0.2641356809703450, -0.1332568242984661, 0, 0.1332568242984661, 0.2641356809703450, 0.3903010380302908, 0.5095014778460075, 0.6196098757636461, 0.7186613631319502, 0.8048884016188399, 0.8767523582704416, 0.9329710868260161, 0.9725424712181152, 0.9947693349975522, 0, 0},
    {-0.9951872199970213, -0.9747285559713095, -0.9382745520027328, -0.8864155270044011, -0.8200019859739029, -0.7401241915785544, -0.6480936519369755, -0.5454214713888396, -0.4337935076260451, -0.3150426796961634, -0.1911188674736163, -0.0640568928626056, 0.0640568928626056, 0.1911188674736163, 0.3150426796961634, 0.4337935076260451, 0.5454214713888396, 0.6480936519369755, 0.7401241915785544, 0.8200019859739029, 0.8864155270044011, 0.9382745520027328, 0.9747285559713095, 0.9951872199970213, 0},
    {-0.9955569697904981, -0.9766639214595175, -0.9429745712289743, -0.8949919978782753, -0.8334426287608340, -0.7592592630373576, -0.6735663684734684, -0.5776629302412229, -0.4730027314457150, -0.3611723058093879, -0.2438668837209884, -0.1228646926107104, 0, 0.1228646926107104, 0.2438668837209884, 0.3611723058093879, 0.4730027314457150, 0.5776629302412229, 0.6735663684734684, 0.7592592630373576, 0.8334426287608340, 0.8949919978782753, 0.9429745712289743, 0.9766639214595175, 0.9955569697904981}
};

static MatrixXd quadrature_weights {
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.5555555555555556, 0.8888888888888888, 0.5555555555555556, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.3478548451374538, 0.6521451548625461, 0.6521451548625461, 0.3478548451374538, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.2369268850561891, 0.4786286704993665, 0.5688888888888889, 0.4786286704993665, 0.2369268850561891, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.1713244923791704, 0.3607615730481386, 0.4679139345726910, 0.4679139345726910, 0.3607615730481386, 0.1713244923791704, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.1294849661688697, 0.2797053914892766, 0.3818300505051189, 0.4179591836734694, 0.3818300505051189, 0.2797053914892766, 0.1294849661688697, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.1012285362903763, 0.2223810344533745, 0.3137066458778873, 0.3626837833783620, 0.3626837833783620, 0.3137066458778873, 0.2223810344533745, 0.1012285362903763, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0812743883615744, 0.1806481606948574, 0.2606106964029354, 0.3123470770400029, 0.3302393550012598, 0.3123470770400029, 0.2606106964029354, 0.1806481606948574, 0.0812743883615744, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0666713443086881, 0.1494513491505806, 0.2190863625159820, 0.2692667193099963, 0.2955242247147529, 0.2955242247147529, 0.2692667193099963, 0.2190863625159820, 0.1494513491505806, 0.0666713443086881, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0556685671161737, 0.1255803694649046, 0.1862902109277343, 0.2331937645919905, 0.2628045445102467, 0.2729250867779006, 0.2628045445102467, 0.2331937645919905, 0.1862902109277343, 0.1255803694649046, 0.0556685671161737, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0471753363865118, 0.1069393259953184, 0.1600783285433462, 0.2031674267230659, 0.2334925365383548, 0.2491470458134028, 0.2491470458134028, 0.2334925365383548, 0.2031674267230659, 0.1600783285433462, 0.1069393259953184, 0.0471753363865118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0404840047653159, 0.0921214998377285, 0.1388735102197872, 0.1781459807619457, 0.2078160475368885, 0.2262831802628972, 0.2325515532308739, 0.2262831802628972, 0.2078160475368885, 0.1781459807619457, 0.1388735102197872, 0.0921214998377285, 0.0404840047653159, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0351194603317519, 0.0801580871597602, 0.1215185706879032, 0.1572031671581935, 0.1855383974779378, 0.2051984637212956, 0.2152638534631578, 0.2152638534631578, 0.2051984637212956, 0.1855383974779378, 0.1572031671581935, 0.1215185706879032, 0.0801580871597602, 0.0351194603317519, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0307532419961173, 0.0703660474881081, 0.1071592204671719, 0.1395706779261543, 0.1662692058169939, 0.1861610000155622, 0.1984314853271116, 0.2025782419255613, 0.1984314853271116, 0.1861610000155622, 0.1662692058169939, 0.1395706779261543, 0.1071592204671719, 0.0703660474881081, 0.0307532419961173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0271524594117541, 0.0622535239386479, 0.0951585116824928, 0.1246289712555339, 0.1495959888165767, 0.1691565193950025, 0.1826034150449236, 0.1894506104550685, 0.1894506104550685, 0.1826034150449236, 0.1691565193950025, 0.1495959888165767, 0.1246289712555339, 0.0951585116824928, 0.0622535239386479, 0.0271524594117541, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0241483028685479, 0.0554595293739872, 0.0850361483171792, 0.1118838471934040, 0.1351363684685255, 0.1540457610768103, 0.1680041021564500, 0.1765627053669926, 0.1794464703562065, 0.1765627053669926, 0.1680041021564500, 0.1540457610768103, 0.1351363684685255, 0.1118838471934040, 0.0850361483171792, 0.0554595293739872, 0.0241483028685479, 0, 0, 0, 0, 0, 0, 0, 0},
    {0.0216160135264833, 0.0497145488949698, 0.0764257302548891, 0.1009420441062872, 0.1225552067114785, 0.1406429146706507, 0.1546846751262652, 0.1642764837458327, 0.1691423829631436, 0.1691423829631436, 0.1642764837458327, 0.1546846751262652, 0.1406429146706507, 0.1225552067114785, 0.1009420441062872, 0.0764257302548891, 0.0497145488949698, 0.0216160135264833, 0, 0, 0, 0, 0, 0, 0},
    {0.0194617882297265, 0.0448142267656996, 0.0690445427376412, 0.0914900216224500, 0.1115666455473340, 0.1287539625393362, 0.1426067021736066, 0.1527660420658597, 0.1589688433939543, 0.1610544498487837, 0.1589688433939543, 0.1527660420658597, 0.1426067021736066, 0.1287539625393362, 0.1115666455473340, 0.0914900216224500, 0.0690445427376412, 0.0448142267656996, 0.0194617882297265, 0, 0, 0, 0, 0, 0},
    {0.0176140071391521, 0.0406014298003869, 0.0626720483341091, 0.0832767415767048, 0.1019301198172404, 0.1181945319615184, 0.1316886384491766, 0.1420961093183820, 0.1491729864726037, 0.1527533871307258, 0.1527533871307258, 0.1491729864726037, 0.1420961093183820, 0.1316886384491766, 0.1181945319615184, 0.1019301198172404, 0.0832767415767048, 0.0626720483341091, 0.0406014298003869, 0.0176140071391521, 0, 0, 0, 0, 0},
    {0.0160172282577743, 0.0369537897708525, 0.0571344254268572, 0.0761001136283793, 0.0934444234560339, 0.1087972991671484, 0.1218314160537285, 0.1322689386333375, 0.1398873947910731, 0.1445244039899700, 0.1460811336496904, 0.1445244039899700, 0.1398873947910731, 0.1322689386333375, 0.1218314160537285, 0.1087972991671484, 0.0934444234560339, 0.0761001136283793, 0.0571344254268572, 0.0369537897708525, 0.0160172282577743, 0, 0, 0, 0},
    {0.0146279952982722, 0.0337749015848142, 0.0522933351526833, 0.0697964684245205, 0.0859416062170677, 0.1004141444428810, 0.1129322960805392, 0.1232523768105124, 0.1311735047870624, 0.1365414983460152, 0.1392518728556320, 0.1392518728556320, 0.1365414983460152, 0.1311735047870624, 0.1232523768105124, 0.1129322960805392, 0.1004141444428810, 0.0859416062170677, 0.0697964684245205, 0.0522933351526833, 0.0337749015848142, 0.0146279952982722, 0, 0, 0},
    {0.0134118594871418, 0.0309880058569794, 0.0480376717310847, 0.0642324214085258, 0.0792814117767189, 0.0929157660600352, 0.1048920914645414, 0.1149966402224114, 0.1230490843067295, 0.1289057221880822, 0.1324620394046966, 0.1336545721861062, 0.1324620394046966, 0.1289057221880822, 0.1230490843067295, 0.1149966402224114, 0.1048920914645414, 0.0929157660600352, 0.0792814117767189, 0.0642324214085258, 0.0480376717310847, 0.0309880058569794, 0.0134118594871418, 0, 0},
    {0.0123412297999872, 0.0285313886289337, 0.0442774388174198, 0.0592985849154368, 0.0733464814110803, 0.0861901615319533, 0.0976186521041139, 0.1074442701159656, 0.1155056680537256, 0.1216704729278034, 0.1258374563468283, 0.1279381953467522, 0.1279381953467522, 0.1258374563468283, 0.1216704729278034, 0.1155056680537256, 0.1074442701159656, 0.0976186521041139, 0.0861901615319533, 0.0733464814110803, 0.0592985849154368, 0.0442774388174198, 0.0285313886289337, 0.0123412297999872, 0},
    {0.0113937985010263, 0.0263549866150321, 0.0409391567013063, 0.0549046959758352, 0.0680383338123569, 0.0801407003350010, 0.0910282619829637, 0.1005359490670506, 0.1085196244742637, 0.1148582591457116, 0.1194557635357848, 0.1222424429903100, 0.1231760537267154, 0.1222424429903100, 0.1194557635357848, 0.1148582591457116, 0.1085196244742637, 0.1005359490670506, 0.0910282619829637, 0.0801407003350010, 0.0680383338123569, 0.0549046959758352, 0.0409391567013063, 0.0263549866150321, 0.0113937985010263}
};

static double eta0DB = 376.73031346;
static float eta0FL = 376.73031346;
static double c = 299792458.0;
static double mu = 4e-7 * M_PI;
static float M_PIFL = (float)M_PI;
static dcomp cuDB(0.0, 1.0);
static fcomp cuFL(0.0f, 1.0f);

static MatrixXd readData(string fileToOpen) {
    ifstream matrixDataFile;
    matrixDataFile.open(fileToOpen);
    if (!matrixDataFile) {
        cout << "Cannot find input file " << fileToOpen << ". The code assumes that it is executed from immediately within the BEMsim3D directory." << endl;
        exit(1);
    }
    vector<double> matrixEntries;
    string matrixRowString;
    string matrixEntry;
    int matrixRowNumber = 0;
    while (getline(matrixDataFile, matrixRowString)) {
        stringstream matrixRowStringStream(matrixRowString);
        while (getline(matrixRowStringStream, matrixEntry, ','))
            matrixEntries.push_back(stod(matrixEntry));
        matrixRowNumber++;
    }
    return Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(matrixEntries.data(), matrixRowNumber, matrixEntries.size() / matrixRowNumber);
}

namespace Eigen {
    template<class Matrix> inline void readBinary(const string& filename, Matrix& matrix) {
        ifstream in(filename, ios::in | ios::binary);
        if (in.is_open()) {
            typename Matrix::Index rows = 0, cols = 0;
            in.read(reinterpret_cast<char*>(&rows), sizeof(typename Matrix::Index));
            in.read(reinterpret_cast<char*>(&cols), sizeof(typename Matrix::Index));
            matrix.resize(rows, cols);
            in.read(reinterpret_cast<char*>(matrix.data()), rows * cols * static_cast<typename Matrix::Index>(sizeof(typename Matrix::Scalar)));
            in.close();
        } else {
            cout << "Cannot open binary matrix file " << filename << ". The code assumes that it is executed from immediately within the BEMsim3D directory." << endl;
            exit(1);
        }
    }
    template<class Matrix> inline void writeBinary(const string& filename, const Matrix& matrix) {
        ofstream out(filename, ios::out | ios::binary | ios::trunc);
        if (out.is_open()) {
            typename Matrix::Index rows = matrix.rows(), cols = matrix.cols();
            out.write(reinterpret_cast<char*>(&rows), sizeof(typename Matrix::Index));
            out.write(reinterpret_cast<char*>(&cols), sizeof(typename Matrix::Index));
            out.write(reinterpret_cast<const char*>(matrix.data()), rows * cols * static_cast<typename Matrix::Index>(sizeof(typename Matrix::Scalar)));
            out.close();
        } else {
            cout << "Cannot write to binary matrix file: " << filename << endl;
        }
    }
    template<class Matrix> inline void writeData(const string& filename, const Matrix& matrix) {
        ofstream out(filename, ios::out | ios::binary | ios::trunc);
        if (out.is_open()) {
            typename Matrix::Index rows = matrix.rows(), cols = matrix.cols();
            out.write(reinterpret_cast<const char*>(matrix.data()), rows * cols * static_cast<typename Matrix::Index>(sizeof(typename Matrix::Scalar)));
            out.close();
        } else {
            cout << "Cannot write to binary matrix file: " << filename << endl;
        }
    }
}