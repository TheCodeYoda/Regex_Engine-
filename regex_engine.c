#include <stdio.h>
// version 3 : 
// 1. empty pattern
// 2. exact match
// 3. anchor ^
// 4. anchor $
// 5. metachar .
// 6. closure *
#define DEBUG 1
int match_here(char *pat, char *text,int *start,int *end);
int match_star(char ch, char* pat, char* text, int *start,int *end);
int match_star_nongreedy(char ch, char* pat, char* text, int *start,int *end);
int match_plus(char ch, char* pat, char* text,int *start,int *end);
int match_plus_nongreedy(char ch, char* pat, char* text, int *start,int *end);
int match_questionmark(char ch, char* pat, char* text,int *start,int *end);
int match_star_digit(char *pat,char *text,int *start, int *end);
int match_star_nongreedy_digit(char *pat,char *text,int *start, int *end);
int match_plus_digit(char *pat,char *text,int *start, int *end);
int match_questionmark_digit(char *pat,char *text,int *start, int *end);
int match_star_word(char *pat,char *text,int *start, int *end);
int match_star_nongreedy_word(char *pat,char *text,int *start, int *end);
int match_plus_word(char *pat,char *text,int *start, int *end);
int match_questionmark_word(char *pat,char *text,int *start, int *end);
int match_star_dot(char *pat,char *text,int *start, int *end);
int match_star_nongreedy_dot(char *pat,char *text,int *start, int *end);
int match_plus_dot(char *pat,char *text,int *start, int *end);
int match_questionmark_dot(char *pat,char *text,int *start, int *end);
int is_az(char ch);
int is_AZ(char ch);
int is_09(char ch);
int is_ch(char ch,char x);
int check_character_class(char *pat, char ch);

int is_az(char ch) {
	if(ch>=97 && ch<=122) return 1;
	return 0;
}

int is_AZ(char ch) {
	if(ch>=65 && ch<=90) return 1;
	return 0;
}

int is_09(char ch) {
	if(ch>=48 && ch<=57) return 1;
	return 0;
}

int is_ch(char ch,char x) {
	if(ch==x) return 1;
	return 0;
}

int match(char *pat,char *text,int *start,int *end)
{
	if(*pat == '^')
	{
		return match_here(pat+1,text,start,end);
	}
	do
	{
		if(match_here(pat,text,start,end)) {
			return 1;
		}
		else {
			(*start)++;
			*end=0;
		}
	} 	while(*text++ != '\0');
	return 0;
}
int match_here(char *pat, char *text,int *start, int *end)
{
	// empty pattern
	if(*pat == '\0')
		return 1;
	// greedy *
	if(pat[0]!='\\' && pat[1] == '*' && pat[2]!='?')
		return match_star(pat[0],pat+2,text,start,end);
	// greedy +
	if(pat[0]!='\\' && pat[1] == '+' && pat[2]!='?')
		return match_plus(pat[0],pat+2,text,start,end);
	// non greedy *
	if(pat[0]!='\\' && pat[2] == '?') {
		if(pat[1]=='*')
			return match_star_nongreedy(pat[0],pat+3,text,start,end);
	}
	if(pat[0]!='\\' && pat[1]=='?' && pat[0]!='*') {
		//printf("pat[0]: %c\n",pat[0]);
		return match_questionmark(pat[0],pat+2,text,start,end);
	}
	if(pat[0] == '$' && pat[1] == '\0')
		return *text == '\0';
	int flag=0;// for character classes
	int flag_1=0; // for \d
	int flag_2=0; // for \w
	int flag_plus=0;
	int flag_star=0;
	int flag_questionmark=0;
	int flag_backslash=0;
	int flag_bracket=0;
	int flag_dot=0;
	int flag_enter=0;
	if(pat[0]=='[') {
		flag_enter=1;
		pat++;
		while(*pat!=']') {
			//printf("pat: %c   flag: %d\n",pat[0],flag);
			if(pat[0]=='a' && pat[1]=='-' && pat[2]=='z') {
				//printf("yo1\n");
				flag|=is_az(*text);
				pat+=2;
			}
			else if(pat[0]=='A' && pat[1]=='-' && pat[2]=='Z') {
				//printf("yo2\n");
				flag|=is_AZ(*text);
				pat+=2;
			}
			else if(pat[0]=='0' && pat[1]=='-' && pat[2]=='9') {
				flag|=is_09(*text);
				pat+=2;
			}
			else
				flag|=is_ch(*text,*pat);
			pat++;
		}
	}
	if(pat[0]=='\\' && pat[1]=='d' && pat[2]=='*' && pat[3]=='?')
		return match_star_nongreedy_digit(pat+4,text,start,end);

	if(pat[0]=='\\' && pat[1]=='d' && pat[2]=='*' && pat[3]!='?')
		return match_star_digit(pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='d' && pat[2]=='+')
		return match_plus_digit(pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='d' && pat[2]=='?')
		return match_questionmark_digit(pat+3,text,start,end);
	
	if(pat[0]=='\\' && pat[1]=='d') {
		flag_enter=1;
		flag_1=is_09(*text);
		pat++;
	}

	if(pat[0]=='\\' && pat[1]=='w' && pat[2]=='*' && pat[3]=='?')
		return match_star_nongreedy_word(pat+4,text,start,end);

	if(pat[0]=='\\' && pat[1]=='w' && pat[2]=='*' && pat[3]!='?')
		return match_star_word(pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='w' && pat[2]=='+')
		return match_plus_word(pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='w' && pat[2]=='?')
		return match_questionmark_word(pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='w') {
		flag_2=is_09(*text)|is_az(*text)|is_AZ(*text)|is_ch(*text,'_');
		flag_enter=1;
		pat++;
	}

	if(pat[0]=='\\' && pat[1]=='*' && pat[2]=='*' && pat[3]=='?')
		return match_star_nongreedy('*',pat+4,text,start,end);

	if(pat[0]=='\\' && pat[1]=='*' && pat[2]=='*' && pat[3]!='?')
		return match_star('*',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='*' && pat[2]=='+')
		return match_plus('*',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='*' && pat[2]=='?')
		return match_questionmark('*',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='*') {
		flag_enter=1;
		flag_star=is_ch(*text,'*');
		pat++;
	}

	if(pat[0]=='\\' && pat[1]=='+' && pat[2]=='*' && pat[3]=='?')
		return match_star_nongreedy('+',pat+4,text,start,end);

	if(pat[0]=='\\' && pat[1]=='+' && pat[2]=='*' && pat[3]!='?')
		return match_star('+',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='+' && pat[2]=='+')
		return match_plus('+',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='+' && pat[2]=='?')
		return match_questionmark('+',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='+') {
		flag_enter=1;
		flag_plus=is_ch(*text,'+');
		pat++;
	}

	if(pat[0]=='\\' && pat[1]=='?' && pat[2]=='*' && pat[3]=='?')
		return match_star_nongreedy('?',pat+4,text,start,end);

	if(pat[0]=='\\' && pat[1]=='?' && pat[2]=='*' && pat[3]!='?')
		return match_star('?',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='?' && pat[2]=='+')
		return match_plus('?',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='?' && pat[2]=='?')
		return match_questionmark('?',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='?') {
		flag_enter=1;
		flag_questionmark=is_ch(*text,'?');
		pat++;
	}

	if(pat[0]=='\\' && pat[1]=='.' && pat[2]=='*' && pat[3]=='?')
		return match_star_nongreedy_dot(pat+4,text,start,end);

	if(pat[0]=='\\' && pat[1]=='.' && pat[2]=='*' && pat[3]!='?')
		return match_star_dot(pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='.' && pat[2]=='+')
		return match_plus_dot(pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='.' && pat[2]=='?')
		return match_questionmark_dot(pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='.') {
		flag_enter=1;
		flag_dot=is_ch(*text,'.');
		pat++;
	}

	if(pat[0]=='\\' && pat[1]=='\\' && pat[2]=='*' && pat[3]=='?')
		return match_star_nongreedy('\\',pat+4,text,start,end);

	if(pat[0]=='\\' && pat[1]=='\\' && pat[2]=='*' && pat[3]!='?')
		return match_star('\\',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='\\' && pat[2]=='+')
		return match_plus('\\',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='\\' && pat[2]=='?')
		return match_questionmark('\\',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='\\') {
		flag_enter=1;
		flag_backslash=is_ch(*text,'\\');
		pat++;
	}

	if(pat[0]=='\\' && pat[1]=='[' && pat[2]=='*' && pat[3]=='?')
		return match_star_nongreedy('[',pat+4,text,start,end);

	if(pat[0]=='\\' && pat[1]=='[' && pat[2]=='*' && pat[3]!='?')
		return match_star('[',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='[' && pat[2]=='+')
		return match_plus('[',pat+3,text,start,end);

	if(pat[0]=='\\' && pat[1]=='[' && pat[2]=='?')
		return match_questionmark('[',pat+3,text,start,end);

	if(*(pat-1)!='\\' && pat[0]=='\\' && pat[1]=='[') {
		flag_enter=1;
		flag_bracket=is_ch(*text,'[');
		pat++;
	}
	// non-greedy charcter class *
	if(pat[0]==']' && pat[1] == '*' && pat[2] == '?') 
		return match_star_nongreedy(pat[0],pat+3,text,start,end);
	// greedy character class *
	if(pat[0]==']' && pat[1] == '*') 
		return match_star(pat[0],pat+2,text,start,end);
	//greedy character class +
	if(pat[0]==']' && pat[1] == '+') 
		return match_plus(pat[0],pat+2,text,start,end);

	if(pat[0]==']' && pat[1] == '?') 
		return match_questionmark(pat[0],pat+2,text,start,end);
	
	if(flag_enter & (flag || flag_1 || flag_2 || flag_star || flag_plus || flag_questionmark || flag_backslash || flag_bracket || flag_dot)) {
		//printf("%d %c\n",flag,*(pat+1));
		(*end)++;
		return match_here(pat+1,text+1,start,end);
	}
	if(!flag_enter && *text != '\0' && (*pat == '.' || *pat == *text))
	{
		//printf("start: %d   end: %d pat:%c  text:%c\n",*start,*end,*pat,*text);
		(*end)++;
		return match_here(pat+1,text+1,start,end);
	}	
	return 0;	
}

int check_character_class(char *pat, char ch) {
	int flag=0;
	int cnt=0;
	pat-=2;
	cnt++;
	while(*pat!='[') {
		//printf("pat: %c   flag: %d\n",pat[0],flag);
		if(*pat=='z' && *(pat-1)=='-' && *(pat-2)=='a') {
			//printf("yo1\n");
			flag|=is_az(ch);
			pat-=2;
			cnt+=2;
		}
		else if(*pat=='Z' && *(pat-1)=='-' && *(pat-2)=='A') {
			//printf("yo2\n");
			flag|=is_AZ(ch);
			pat-=2;
			cnt+=2;
		}
		else if(*pat=='9' && *(pat-1)=='-' && *(pat-2)=='0') {
			flag|=is_09(ch);
			pat-=2;
			cnt+=2;
		}
		else
			flag|=is_ch(ch,*pat);
		pat--;
		cnt+=1;
	}
	pat+=cnt;
	pat++;
	return flag;
}

int match_star(char ch, char* pat, char* text,int *start, int *end)
{
    int flag=0;
    if(ch==']') {
        flag=check_character_class(pat,*text);
    }
    char *initial_text = text;
    int text_i = 0;
    while(text[text_i] != '\0' && (text[text_i] == ch || ch == '.' || flag)) {
        //printf("flag: %d char: %c\n",flag,*text);
        //(*end)++;
        text_i++;
        if(ch==']')
            flag=check_character_class(pat,text[text_i]);
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i >= 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_star_nongreedy(char ch, char *pat, char *text, int *start, int *end) {
	int flag;
	char *initial_text=text;
	int text_i=0;
	int initial_end=*end;
	do
	{
		flag=0;
		*end=initial_end;
		if(ch==']')
			flag=check_character_class(pat,text[text_i]);
		if(match_here(pat,&text[text_i],start,end)) {
			(*end)+=text_i;
			return 1;
		}
	} while(text[text_i] != '\0' && (text[text_i++] == ch || ch == '.' || flag));
	return 0;
}

int match_plus(char ch,char *pat, char *text,int *start, int *end) {
	int flag=0;
    if(ch==']') {
        flag=check_character_class(pat,*text);
    }
    char *initial_text = text;
    int text_i = 0;
    while(text[text_i] != '\0' && (text[text_i] == ch || ch == '.' || flag)) {
        //printf("flag: %d char: %c\n",flag,*text);
        //(*end)++;
        text_i++;
        if(ch==']')
            flag=check_character_class(pat,text[text_i]);
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i > 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_questionmark(char ch, char *pat, char* text,int *start,int *end) {
	//printf("yo %c %c %c\n",*(text+1),*text,*pat);
	int flag=0;
    if(ch==']') {
        flag=check_character_class(pat,*text);
    }
    char *initial_text = text;
    int text_i = 0;
    if(text[text_i] != '\0' && (text[text_i] == ch || ch == '.' || flag)) {
        //printf("flag: %d char: %c\n",flag,*text);
        //(*end)++;
        text_i++;
        if(ch==']')
            flag=check_character_class(pat,text[text_i]);
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i >= 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_star_digit(char* pat, char* text,int *start, int *end)
{
    char *initial_text = text;
    int text_i = 0;
    while(text[text_i] != '\0' && (is_09(text[text_i]))) {
        //printf("flag: %d char: %c\n",flag,*text);
        //(*end)++;
        text_i++;
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i >= 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_star_nongreedy_digit(char *pat, char *text, int *start, int *end) {
	char *initial_text=text;
	int text_i=0;
	int initial_end=*end;
	do
	{
		*end=initial_end;
		if(match_here(pat,&text[text_i],start,end)) {
			(*end)+=text_i;
			return 1;
		}
	} while(text[text_i] != '\0' && is_09(text[text_i++]));
	return 0;
}

int match_plus_digit(char *pat, char *text,int *start, int *end) {
    char *initial_text = text;
    int text_i = 0;
    while(text[text_i] != '\0' && (is_09(text[text_i]))) {
        //printf("flag: %d char: %c\n",flag,*text);
        //(*end)++;
        text_i++;
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i > 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_questionmark_digit(char *pat, char* text,int *start,int *end) {
	char *initial_text = text;
    int text_i = 0;
    if(text[text_i] != '\0' && (is_09(text[text_i]))) {
        //printf("flag: %d char: %c\n",flag,*text);
        //(*end)++;
        text_i++;
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i >= 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_star_word(char* pat, char* text,int *start, int *end)
{
    char *initial_text = text;
    int text_i = 0;
    while(text[text_i] != '\0' && (is_09(text[text_i])|is_az(text[text_i])|is_AZ(text[text_i])|is_ch(text[text_i],'_'))) {
        //printf("char: %c %c\n",text[text_i],*pat);
        //(*end)++;
        text_i++;
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i >= 0) {
      *end = initial_end;
      //printf("%c %c\n",*pat,text[text_i]);
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
	      	//printf("in star end after second loop: %d\n",*end);
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_star_nongreedy_word(char *pat, char *text, int *start, int *end) {
	int flag;
	char *initial_text=text;
	int text_i=0;
	int initial_end=*end;
	do
	{
		*end=initial_end;
		if(match_here(pat,&text[text_i],start,end)) {
			(*end)+=text_i;
			return 1;
		}
		flag=(is_09(text[text_i])|is_az(text[text_i])|is_AZ(text[text_i])|is_ch(text[text_i],'_'));
		text_i++;
	} while(text[text_i] != '\0' && flag);
	return 0;
}

int match_plus_word(char *pat, char *text,int *start, int *end) {
    char *initial_text = text;
    int text_i = 0;
    while(text[text_i] != '\0' && (is_09(text[text_i])|is_az(text[text_i])|is_AZ(text[text_i])|is_ch(text[text_i],'_'))) {
        //printf("char: %c\n",text[text_i]);
        //(*end)++;
        text_i++;
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i > 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_questionmark_word(char *pat, char* text,int *start,int *end) {
	char *initial_text = text;
    int text_i = 0;
    if(text[text_i] != '\0' && (is_09(text[text_i])|is_az(text[text_i])|is_AZ(text[text_i])|is_ch(text[text_i],'_'))) {
        //printf("char: %c\n",text[text_i]);
        //(*end)++;
        text_i++;
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i >= 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_star_dot(char* pat, char* text,int *start, int *end)
{
    char *initial_text = text;
    int text_i = 0;
    while(text[text_i] != '\0' && text[text_i]=='.') {
        //printf("flag: %d char: %c\n",flag,*text);
        //(*end)++;
        text_i++;
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i >= 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_star_nongreedy_dot(char *pat, char *text, int *start, int *end) {
	char *initial_text=text;
	int text_i=0;
	int initial_end=*end;
	do
	{
		*end=initial_end;
		if(match_here(pat,&text[text_i],start,end)) {
			(*end)+=text_i;
			return 1;
		}
	} while(text[text_i] != '\0' && text[text_i]=='.');
	return 0;
}

int match_plus_dot(char *pat, char *text,int *start, int *end) {
    char *initial_text = text;
    int text_i = 0;
    while(text[text_i] != '\0' && text[text_i]=='.') {
        //printf("flag: %d char: %c\n",flag,*text);
        //(*end)++;
        text_i++;
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i > 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}

int match_questionmark_dot(char *pat, char* text,int *start,int *end) {
	char *initial_text = text;
    int text_i = 0;
    if(text[text_i] != '\0' && text[text_i]=='.') {
        //printf("flag: %d char: %c\n",flag,*text);
        //(*end)++;
        text_i++;
    }
    //printf("end after first loop: %d\n",*end);

    int initial_end = *end;
    while (text_i >= 0) {
      *end = initial_end;
        if(match_here(pat,&text[text_i],start,end)) {
        	(*end)+=text_i;
            return 1;
        }
        //(*end)--;
        text_i--;
    }
    //printf("end after second loop: %d\n",*end);
    return 0;
}


int main()
{
	char text[4096];
	scanf("%[^\n]",text);
	int m;
	scanf("%d",&m);
	while(m--) {
		char pat[1024];
		char c;
		while ( (c = getchar()) != '\n' && c != EOF );
		scanf("%[^\n]",pat);
		int start,end;
		start=0;
		end=0;
		if(match(pat,text,&start,&end)) {
			if(start==0 && end==0)
				printf("%d %d %d\n",1,start,end);
			else {
				//printf("%d %d\n",start,end);
				printf("%d %d %d\n",1,start,start+end-1);
			}
		}
		else
			printf("%d\n",0);
	}
}

