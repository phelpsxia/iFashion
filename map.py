# Empty dict
map = {}
# Fill in the entries one by one
map[b'e2009000170400830410ec6a'] = 'P000BKIB'
map[b'e2009000170400830430ec6c'] = 'P000BKIO'
map[b'e2009000170400830490e642'] = 'P000BKFL'
map[b'e2009000170400830480e641'] = 'P000BKEH'
map[b'e2009000170400830420ec6b'] = 'P000BKCW'
map[b'e2009000170400830470e750'] = 'P000BKBB'
map[b'e2009000170400830440e74d'] = 'P000BKAW'
map[b'e2009000170400830450e74e'] = 'P000BJTH'
map[b'e2009000170400830460e74f'] = 'P000BJNJ'

def mapping(tagId):
    itemId = map[tagId]
    return itemId

if __name__ == "__main__":
    a = mapping(b'e2009000170400830460e74f')
    print(a)

