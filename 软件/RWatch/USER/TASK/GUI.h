#ifndef __GUI_H
#define __GUI_H

//��������в˵��л�

#define MAX_MENU_FIR_LEVEL 3    //һ���˵��������
#define MAX_MENU_SEC_LEVEL 2	  //�����˵��������
//#define MAX_MENU_THR_LEVEL 2


typedef struct
{
		unsigned char index;        // 1 ,��ŵ�ǰ�����������
		unsigned char down_index; 	// 2 ,���¡�down�����£�����ʱ��Ҫ��ת���������� 
		unsigned char up_index;     // 3 ,���¡�up�����ϣ�����ʱ��Ҫ��ת���������ţ� 
		unsigned char enter_index;  // 4 ,���¡�enter�����룩����ʱ��Ҫ��ת�������ţ� 
		unsigned char esc_index;    // 5 ,��ǰ����������Ҫִ�� �ĺ�������ڵ�ַ�� 
		void (*operate)(void);  				// 6 ,����ָ��
}Menu_index_struct;


///*һ���˵�ö��*/
//typedef enum
//{
//    INIT,           //��ʼ������
//    MAIN,           //ʱ����ҳ��
//    REMOTE_MONITOR, //�����ҳ��
//    GYRO_MONITOR,   //�����Ǽ���ҳ��
//    MENU,           //�˵�
//    TWO_LEVEL_MENU, //�����˵������ԣ����滻�ɾ��幦�ܲ˵���

//} MENU_STATE;


//typedef struct
//{
//    unsigned int menu_state[4,];        //����״̬��1/0��
//    int menu_flag;             //�˵���
//    int oled_switch;           //�л�ҳ��ʱˢ����Ļ��־λ

//    int option_flag;           //��ǰѡ����
//    int last_option_flag;      //�ϴ����·������ݣ�ѡ�����·�ֱ�Ӽ�

//} OLED_DATA_TYPE_DEF;


//�ṹ��ͱ�������
//extern GUI_STATE GUI_state;       //ͼ���û�����ö��
extern Menu_index_struct menu_index;

void Menu_State_Switch(void);

#endif
