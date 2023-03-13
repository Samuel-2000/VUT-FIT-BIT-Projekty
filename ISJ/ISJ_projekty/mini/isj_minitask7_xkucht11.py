def tags(tag_name):
    def tags_decorator(func):
        def inner(name):
            return '<{0}>{1}</{0}>'.format(tag_name, func(name))
        return inner
    return tags_decorator

@tags('strong')
def get_text(name):
    return 'Hello '+name

get_text = tags('p')(get_text)
print(get_text('John'))