import sys
from dataclasses import dataclass, field


@dataclass
class Vertex:
    x: float = 0.0
    y: float = 0.0
    z: float = 0.0


@dataclass
class Index:
    v_index: int
    n_index: int


@dataclass
class Triangle:
    v1: Index
    v2: Index
    v3: Index


@dataclass
class Mesh:
    vertices: list[Vertex] = field(default_factory=list)
    normals: list[Vertex] = field(default_factory=list)
    triangles: list[Triangle] = field(default_factory=list)


def read_obj(filename):
    m = Mesh()
    with open(filename, 'r') as infile:
        for line in infile:
            if line.startswith('v '):
                x = float(line.split(' ')[1])
                y = float(line.split(' ')[2])
                z = float(line.split(' ')[3])
                m.vertices.append(Vertex(x=x, y=y, z=z))
            elif line.startswith('vn '):
                x = float(line.split(' ')[1])
                y = float(line.split(' ')[2])
                z = float(line.split(' ')[3])
                m.normals.append(Vertex(x=x, y=y, z=z))
            elif line.startswith('f '):
                v1 = line.strip().split(' ')[1]
                v1i = v1.split('/')[0]
                n1i = v1.split('/')[2]
                v2 = line.strip().split(' ')[2]
                v2i = v2.split('/')[0]
                n2i = v2.split('/')[2]
                v3 = line.strip().split(' ')[3]
                v3i = v3.split('/')[0]
                n3i = v3.split('/')[2]
                m.triangles.append(Triangle(
                    v1=Index(v_index=int(v1i), n_index=int(n1i)),
                    v2=Index(v_index=int(v2i), n_index=int(n2i)),
                    v3=Index(v_index=int(v3i), n_index=int(n3i)),
                ))
    return m


def print_cpp(mesh: Mesh):
    # Print vertices array
    data = '    '
    count = 0
    for v in mesh.vertices:
        data += f'{v.x}, {v.y}, {v.z},'
        count += 1
        if count == 3:
            data += '\n    '
            count = 0
        else:
            data += ' '
    print(f'static const float vertices[] = {{\n{data}\n}};')

    print()

    # Print normals array
    data = '    '
    count = 0
    for n in mesh.normals:
        data += f'{n.x}, {n.y}, {n.z},'
        count += 1
        if count == 3:
            data += '\n    '
            count = 0
        else:
            data += ' '
    print(f'static const float normals[] = {{\n{data}\n}};')
    
    print()

    # Print triangles array
    data = '    '
    count = 0
    for t in mesh.triangles:
        data += f'{t.v1.v_index-1}, {t.v1.n_index-1}, '
        data += f'{t.v2.v_index-1}, {t.v2.n_index-1}, '
        data += f'{t.v3.v_index-1}, {t.v3.n_index-1},'
        count += 1
        if count == 3:
            data += '\n    '
            count = 0
        else:
            data += ' '
    print(f'static const uint16_t triangles[] = {{\n{data}\n}};')


if __name__ == '__main__':
    mesh = read_obj(sys.argv[1])
    print_cpp(mesh)
