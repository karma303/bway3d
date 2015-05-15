union matrix{
	int mm[2][2];
	int mm00,mm01,mm10,mm11;
};

void main(void){
	union matrix m={0,1,2,3};
	union matrix m1={
		{
			{0,1},
			{2,3}
		}
	};
}
