struct hashTable {
    struct ListNode* key;
    UT_hash_handle hh;
};
bool hasCycle(struct ListNode* head) {
    struct hashTable* hashtable = NULL; while (head != NULL) {
        struct hashTable* dup;
        HASH_FIND_PTR(hashtable, &head, dup);
        if (dup != NULL) {
            return true;
        }
        struct hashTable* tmp = malloc(sizeof(struct hashTable));
        tmp->key = head;
        HASH_ADD_PTR(hashtable, key, tmp);
        head = head->next;
    }
    return false;
}
