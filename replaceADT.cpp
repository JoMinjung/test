#include "replace.h"

void replacementSelection()
{
	int input;
	ifstream inStream;
	inStream.open("input.txt");						//�Է� ȭ��

	ofstream outStream;
	outStream.open("run.txt");						//���ĵ� ���� ȭ��(�� ���� ȭ��)

	for(int i=0; i<m; i++)
	{
		inStream>>buffer[i];						//���ۿ� �Է� ȭ�Ϸ� ���� m���� ���ڵ� �ǵ�
	}
	
	while(!inStream.eof())
	{
		int idx_frozen=0;
		while(idx_frozen<m)							//������ ���ڵ� ���� ������ ���ڵ� ���� ������ ������ �ϳ��� �� ����
		{
			if(inStream.eof())
				break;
			inStream>>input;						//�Է� ȭ�Ͽ��� ���� ���ڵ带 �ǵ�
			cout<<"input: "<<input<<endl;			
			int key = min(buffer);					//���ۿ��� Ű ���� ���� ���� ���ڵ带 �����Ͽ� ����
			int p = index(buffer,key);				//���ۿ��� ���� ���ڵ尡 ��ġ�� �ε���
			
			if(input>key)							//�Է� ȭ�Ͽ��� �о��� ���ڵ��� ���ۿ��� ������ ���ڵ��� ��ü
			{
				run[idx_run++] = buffer[p];			//���ۿ��� ������ ���ڵ��� ���� ���ڵ��� ����
				//idx_run++;
				buffer[p] = input;
			}
			else									//�Է� ȭ�Ͽ��� �о��� ���ڵ尡 ���ۿ��� ������ ���ڵ庸�� ��������
			{
				frozen[idx_frozen++] = input;		//�Է� ȭ�Ͽ��� �о��� ���ڵ忡 ����"frozen"ǥ��
				//idx_frozen++;
				run[idx_run++] = buffer[p];			//�� �� ���ۿ��� ������ ���ڵ��� ���� ���ڵ��� ����
				buffer[p] = -1;						//���ۿ��� �� ���ڵ��� Ű ���� ���� ���� ���ڵ带 ������ �� ���� 

			}
		} 
		if(idx_frozen!=0)
			cout<<"frozen of run "<<cnt_run<<endl;
		for(int i=0; i<idx_frozen; i++)			//�� ������ ����"frozen"���ڵ� Ȯ��
			cout<<frozen[i]<<endl;

		if(idx_frozen!=m)							//�Է�ȭ���� ���ڵ带 ���� �ǵ��Ͽ� ��ü������ ��������
		{											//���ۿ� �����ִ� ������ ���ڵ�����
			//int n = min(buffer);					//���������Ͽ� ���� ���� ���ڵ带 �߰�
			//while(n!=-1){
			//	int q = index(buffer,n);
			//	run[idx_run++] = buffer[q];
			//	buffer[q]=-1;
			//	n = min(buffer);}
			sort(buffer);
		}
		writeRun(outStream);						//���� ȭ�Ϸ� ����
			
		for(int i=0; i<idx_run; i++)				//���ο� �� ������ ���� �� �������� �ʱ�ȭ
			run[i] = 0;
		idx_run=0;									//�� ���� ������ �ʱ�ȭ

		if(idx_frozen==m){							//���ο� �� ������ ����
			for(int i=0; i<m; i++){
				buffer[i] = frozen[i];				//������ ���ڵ带 ���� ����
				frozen[i] = 0;}}					//������ ���ڵ� ���� ���� �ʱ�ȭ
		
		else if(idx_frozen==0)						//�Է�ȭ���� ���ڵ带 ���� �ǵ��Ͽ� ��ü����
			break;									//���� ���� ���� ������ ������ ���ڵ尡 ���� ����

		else{										//�Է�ȭ���� ���ڵ带 ���� �ǵ��Ͽ� ��ü������ ���� ����
			sort(frozen);							//�����ִ� ������ ���ڵ����� ���� �����Ͽ� ���� ���� ���ڵ带 �߰�
			writeRun(outStream);					//���� ȭ�Ϸ� ����
		}


	}
}

void writeRun(ofstream& outStream)					//�� ���������� �ִ� ���ڵ����� ȭ�Ϸ� �������ִ� �Լ�
{
	outStream<<"run "<<cnt_run++<<": ";
	for(int i=0; i<idx_run; i++)
		outStream<<run[i]<<" ";
	outStream<<endl;
}
void sort(int b[])								//��ü ������ ���� ���� ���� �Ǵ� ������ ���ڵ带 ���������ϴ� �Լ�
{
	int temp;
	for(int i=0; i<m; i++)
	{
		for(int j=0; j<m-(i+1);  j++)
		{
			if(b[j]>b[j+1])
			{
				temp=b[j+1];
				b[j+1]=b[j];
				b[j]=temp;
			}
		}
	}
	for(int i=0; i<m ;i++)
		if(b[i]!=0 && b[i]!=-1)
			run[idx_run++] = b[i];
}

int index(int b[],int key)						//������ �ּҰ��� ������ ���ڵ��� ��ġ�� ã�� �Լ�
{
	int idx;
	for(int i=0; i<m; i++)
	{
		if(b[i] == key)
			idx = i;
	}
	return idx;
}
int min(int b[])									//������ �ּҰ��� ������ ���ڵ带 ã�� �Լ�
{
	int temp;
	for(int i=0; i<m; i++)
	{
		for(int j=0; j<m-(i+1);  j++)				//���� ������ �����Ͽ� �������� ���� ��
		{
			if(b[j]>b[j+1])
			{
				temp=b[j+1];
				b[j+1]=b[j];
				b[j]=temp;
			}
		}
	}
	int i=0;
	for(i=0; i<m; i++)								//�ּҰ��� ������ ���ڵ带 ������ �� ���ܵ� ���ڵ带 �ǵ��Ͽ�
	{
		if(b[i]!=-1){
			temp = b[i];							//�ּҰ��� ������ ���ڵ带 �ǵ�
			break;}
	}
	if(i==m)										//���ۿ� �ǵ��� ���ڵ尡 ���� ���� 
		return -1;
	return temp;									//�ּҰ��� ������ ���ڵ带 ��ȯ
}

void main()
{
	replacementSelection();
	cout<<"edit";
}
