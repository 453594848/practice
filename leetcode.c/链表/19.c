struct ListNode* removeNthFromEnd(struct ListNode* head, int n){
struct ListNode* pow=head;
int m=0;
while(pow!=NULL){
m++;
pow=pow->next;
}
if(m==1)
return 0;
pow=head;
struct ListNode* dummy = malloc(sizeof(struct ListNode));
    dummy->val = 0, dummy->next = head;
    struct ListNode* cur = dummy;
    for (int i = 0; i < m - n - 1; ++i) {
        cur = cur->next;
    }
    cur->next = cur->next->next;
    struct ListNode* ans = dummy->next;
    free(dummy);
    return ans;
}
