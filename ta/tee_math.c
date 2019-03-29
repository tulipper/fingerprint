#include "tee_math.h"
double tee_abs(double a) {
    return (a >= 0? a : -a);
}
double tee_sqrt(double x) {
    double val = x;//最终
    double eps = 0.0000001;
    double last;//保存上一个计算的值
    do
    {
        last = val;
        val =(val + x/val) / 2;
    }while(tee_abs(val-last) > eps);
    return val;

}

double tee_exp(double x) {
	int i,k,m,t;
	int xm=(int)x; 
	double sum;
	double e ;
	double ef;
	double z ;
	double sub=x-xm;
	m=1;      //阶乘算法分母
	e=1.0;  //e的xm
	ef=1.0;  
	t=10;      //算法精度
	z=1;  //分子初始化
	sum=1;   
	//  printf("x=%f\n",x);
	//  printf("sub=%f\n",sub);
	if (xm<0) {     //判断xm是否大于0？
		xm=(-xm); 
		for(k=0;k<xm;k++){ef*=2.718281;}
		e/=ef;
	} 
	else { for(k=0;k<xm;k++){e*=2.718281;} }
	//  printf("e=%f\n",e);
	//  printf("xm=%d\n",xm);
 	for(i=1;i<t;i++){
	m*=i; 
	z*=sub;  
	sum+=z/m;
	}
	return sum*e;
}
