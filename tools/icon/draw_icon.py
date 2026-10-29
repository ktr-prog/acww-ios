# The app's icon (ios/icon.png, 1024 x 1024): a peach on a polka-dot background, drawn here (an original picture;
# Pillow). python3 tools/icon/draw_icon.py, run from tools/icon, writes icon.png and preview.png there.
from PIL import Image, ImageDraw, ImageFilter, ImageChops
import math
S=4; N=1024; W=N*S
def lerp(a,b,t): return tuple(int(a[i]+(b[i]-a[i])*t) for i in range(len(a)))
# background: soft cream-to-mint vertical gradient
bg=Image.new('RGB',(W,W))
top=(255,244,214); bot=(178,226,176)
px=bg.load()
grad=Image.new('RGB',(1,W))
for y in range(W): grad.putpixel((0,y),lerp(top,bot,(y/W)**1.2))
bg=grad.resize((W,W))
# polka dots, staggered
dots=Image.new('L',(W,W),0); d=ImageDraw.Draw(dots)
step=W//7
for j in range(-1,9):
    for i in range(-1,9):
        cx=i*step+(step//2 if j%2 else 0); cy=j*step
        r=step*0.16
        d.ellipse((cx-r,cy-r,cx+r,cy+r),fill=70)
bg=Image.composite(Image.new('RGB',(W,W),(255,255,255)),bg,dots)
img=bg.convert('RGBA')
# ground shadow
sh=Image.new('L',(W,W),0); ImageDraw.Draw(sh).ellipse((W*0.24,W*0.80,W*0.76,W*0.90),fill=110)
sh=sh.filter(ImageFilter.GaussianBlur(W*0.02))
img=Image.composite(Image.new('RGBA',(W,W),(90,120,70,255)),img,sh)
# fruit: a round peach-orange with radial shading
cx,cy,R=W*0.5,W*0.56,W*0.29
fr=Image.new('RGBA',(W,W),(0,0,0,0))
mask=Image.new('L',(W,W),0); ImageDraw.Draw(mask).ellipse((cx-R,cy-R*0.97,cx+R,cy+R*1.0),fill=255)
# shading via distance from a light point
sm=256
shade=Image.new('RGB',(sm,sm)); sp=shade.load()
lx,ly=0.36,0.30
for y in range(sm):
    for x in range(sm):
        u=x/sm; v=y/sm
        dd=math.hypot(u-lx,v-ly)/0.95
        t=min(1,dd)
        c=lerp((255,196,120),(232,92,58),t**1.1)
        c=lerp(c,(190,60,48),max(0,(t-0.75)/0.25)*0.6)
        sp[x,y]=c
shade=shade.resize((int(2*R),int(2*R)),Image.BICUBIC)
fr.paste(shade,(int(cx-R),int(cy-R)))
img=Image.composite(fr,img,mask)
# crease line
cr=Image.new('L',(W,W),0); ImageDraw.Draw(cr).arc((cx-R*0.55,cy-R*0.95,cx+R*0.35,cy+R*0.9),200,330,fill=90,width=int(W*0.012))
cr=cr.filter(ImageFilter.GaussianBlur(W*0.006))
cr=ImageChops.multiply(cr,mask)
img=Image.composite(Image.new('RGBA',(W,W),(200,70,50,255)),img,cr)
# highlight
hl=Image.new('L',(W,W),0); ImageDraw.Draw(hl).ellipse((cx-R*0.62,cy-R*0.72,cx-R*0.18,cy-R*0.35),fill=150)
hl=hl.filter(ImageFilter.GaussianBlur(W*0.02))
img=Image.composite(Image.new('RGBA',(W,W),(255,250,235,255)),img,hl)
# stem
st=Image.new('L',(W,W),0); ImageDraw.Draw(st).line((cx+W*0.005,cy-R*0.88,cx+W*0.03,cy-R*1.22),fill=255,width=int(W*0.035))
img=Image.composite(Image.new('RGBA',(W,W),(120,78,44,255)),img,st)
# leaf: bezier-ish shape via polygon
def leafpoly(x0,y0,L,Wd,ang,n=80):
    pts=[]
    for k in range(n+1):
        t=k/n; x=L*t; y=Wd*math.sin(math.pi*t)**0.9
        pts.append((x,y))
    for k in range(n,-1,-1):
        t=k/n; x=L*t; y=-Wd*math.sin(math.pi*t)**0.9*0.85
        pts.append((x,y))
    ca,sa=math.cos(ang),math.sin(ang)
    return [(x0+x*ca-y*sa,y0+x*sa+y*ca) for x,y in pts]
lp=leafpoly(cx+W*0.03,cy-R*1.12,W*0.30,W*0.085,math.radians(-28))
lm=Image.new('L',(W,W),0); ImageDraw.Draw(lm).polygon(lp,fill=255)
lg=Image.new('RGB',(W,W)); gl=ImageDraw.Draw(lg)
for k in range(64):
    t=k/63; c=lerp((150,214,88),(64,150,62),t)
    gl.rectangle((0,int(W*0.0+t*W*0.5),W,int(W*0.0+(t+1/63)*W*0.5)+1),fill=c)
lg=lg.transform((W,W),Image.AFFINE,(1,0,0,0,1,-W*0.05))
img=Image.composite(lg.convert('RGBA'),img,lm)
vein=Image.new('L',(W,W),0)
a=math.radians(-28); x0,y0=cx+W*0.03,cy-R*1.12
ImageDraw.Draw(vein).line((x0,y0,x0+W*0.26*math.cos(a),y0+W*0.26*math.sin(a)),fill=200,width=int(W*0.009))
vein=ImageChops.multiply(vein,lm)
img=Image.composite(Image.new('RGBA',(W,W),(210,240,170,255)),img,vein)
out=img.convert('RGB').resize((N,N),Image.LANCZOS)
out.save('icon.png')
# preview with iOS-like rounded mask
m=Image.new('L',(N,N),0); ImageDraw.Draw(m).rounded_rectangle((0,0,N-1,N-1),radius=int(N*0.2237),fill=255)
pv=Image.new('RGB',(N,N),(240,240,240)); pv.paste(out,(0,0),m); pv.resize((512,512),Image.LANCZOS).save('preview.png')
