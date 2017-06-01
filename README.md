int GetIteratorPos(List list) {
    assert(list != NULL);
    if(list -> list_iterator == NULL) {
        return 0;
    }
    Node temp = list -> list_head;
    int i = 0;
    while(temp != list -> list_iterator) {
        temp = temp -> next;
        i++;
    }
    return i;
}



 int iterator_pos = GetIteratorPos(list);
    
    
    list -> list_iterator = list -> list_head;
    while(iterator_pos > 0) {
        list -> list_iterator = list -> list_iterator -> next;
        iterator_pos--;
    }