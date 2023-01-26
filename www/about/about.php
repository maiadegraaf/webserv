#!/usr/bin/php

<!DOCTYPE html>
<html lang="en">
<head>
    <link rel="icon" href="data:,">
    <link rel="stylesheet" href="../css/css.css">
    <meta charset="UTF-8">
    <link rel="shortcut icon" type="image/x-icon" href="../photo/icon.png">
    <title>about us</title>
        <link rel="shortcut icon" type="image/x-icon" href="photo/icon.png">
        <title>~welcome~</title>
        <script type="text/javascript">
            // <![CDATA[
            var colour="#FFFFFF";
            var sparkles=120;

            /****************************
             *  Tinkerbell Magic Sparkle *
             * (c) 2005 mf2fm web-design *
             *  http://www.mf2fm.com/rv  *
             * DON'T EDIT BELOW THIS BOX *
             ****************************/
            var x=ox=400;
            var y=oy=300;
            var swide=800;
            var shigh=600;
            var sleft=sdown=0;
            var tiny=new Array();
            var star=new Array();
            var starv=new Array();
            var starx=new Array();
            var stary=new Array();
            var tinyx=new Array();
            var tinyy=new Array();
            var tinyv=new Array();

            window.onload=function() { if (document.getElementById) {
                var i, rats, rlef, rdow;
                for (var i=0; i<sparkles; i++) {
                    var rats=createDiv(3, 3);
                    rats.style.visibility="hidden";
                    document.body.appendChild(tiny[i]=rats);
                    starv[i]=0;
                    tinyv[i]=0;
                    var rats=createDiv(5, 5);
                    rats.style.backgroundColor="transparent";
                    rats.style.visibility="hidden";
                    var rlef=createDiv(1, 5);
                    var rdow=createDiv(5, 1);
                    rats.appendChild(rlef);
                    rats.appendChild(rdow);
                    rlef.style.top="2px";
                    rlef.style.left="0px";
                    rdow.style.top="0px";
                    rdow.style.left="2px";
                    document.body.appendChild(star[i]=rats);
                }
                set_width();
                sparkle();
            }}

            function sparkle() {
                var c;
                if (x!=ox || y!=oy) {
                    ox=x;
                    oy=y;
                    for (c=0; c<sparkles; c++) if (!starv[c]) {
                        star[c].style.left=(starx[c]=x)+"px";
                        star[c].style.top=(stary[c]=y)+"px";
                        star[c].style.clip="rect(0px, 5px, 5px, 0px)";
                        star[c].style.visibility="visible";
                        starv[c]=50;
                        break;
                    }
                }
                for (c=0; c<sparkles; c++) {
                    if (starv[c]) update_star(c);
                    if (tinyv[c]) update_tiny(c);
                }
                setTimeout("sparkle()", 40);
            }

            function update_star(i) {
                if (--starv[i]==25) star[i].style.clip="rect(1px, 4px, 4px, 1px)";
                if (starv[i]) {
                    stary[i]+=1+Math.random()*3;
                    if (stary[i]<shigh+sdown) {
                        star[i].style.top=stary[i]+"px";
                        starx[i]+=(i%5-2)/5;
                        star[i].style.left=starx[i]+"px";
                    }
                    else {
                        star[i].style.visibility="hidden";
                        starv[i]=0;
                        return;
                    }
                }
                else {
                    tinyv[i]=50;
                    tiny[i].style.top=(tinyy[i]=stary[i])+"px";
                    tiny[i].style.left=(tinyx[i]=starx[i])+"px";
                    tiny[i].style.width="2px";
                    tiny[i].style.height="2px";
                    star[i].style.visibility="hidden";
                    tiny[i].style.visibility="visible"
                }
            }

            function update_tiny(i) {
                if (--tinyv[i]==25) {
                    tiny[i].style.width="1px";
                    tiny[i].style.height="1px";
                }
                if (tinyv[i]) {
                    tinyy[i]+=1+Math.random()*3;
                    if (tinyy[i]<shigh+sdown) {
                        tiny[i].style.top=tinyy[i]+"px";
                        tinyx[i]+=(i%5-2)/5;
                        tiny[i].style.left=tinyx[i]+"px";
                    }
                    else {
                        tiny[i].style.visibility="hidden";
                        tinyv[i]=0;
                        return;
                    }
                }
                else tiny[i].style.visibility="hidden";
            }

            document.onmousemove=mouse;
            function mouse(e) {
                set_scroll();
                y=(e)?e.pageY:event.y+sdown;
                x=(e)?e.pageX:event.x+sleft;
            }

            function set_scroll() {
                if (typeof(self.pageYOffset)=="number") {
                    sdown=self.pageYOffset;
                    sleft=self.pageXOffset;
                }
                else if (document.body.scrollTop || document.body.scrollLeft) {
                    sdown=document.body.scrollTop;
                    sleft=document.body.scrollLeft;
                }
                else if (document.documentElement && (document.documentElement.scrollTop || document.documentElement.scrollLeft)) {
                    sleft=document.documentElement.scrollLeft;
                    sdown=document.documentElement.scrollTop;
                }
                else {
                    sdown=0;
                    sleft=0;
                }
            }

            window.onresize=set_width;
            function set_width() {
                if (typeof(self.innerWidth)=="number") {
                    swide=self.innerWidth;
                    shigh=self.innerHeight;
                }
                else if (document.documentElement && document.documentElement.clientWidth) {
                    swide=document.documentElement.clientWidth;
                    shigh=document.documentElement.clientHeight;
                }
                else if (document.body.clientWidth) {
                    swide=document.body.clientWidth;
                    shigh=document.body.clientHeight;
                }
            }

            function createDiv(height, width) {
                var div=document.createElement("div");
                div.style.position="absolute";
                div.style.height=height+"px";
                div.style.width=width+"px";
                div.style.overflow="hidden";
                div.style.backgroundColor=colour;
                return (div);
            }
            // ]]>
        </script>
</head>
    <body>
        <div class="subtitle-container">
            <div class="wordart blues">
                <span class="text">about us</span>
            </div>
        </div>

        <div class="center-text-box">
            <div class="box background">
                <?php echo
                    "&emsp;Alfred, Storm and Maia were all born and raised in a small fishing village on the coast. The village was isolated from the rest of the world and the only way to communicate with the outside was by ship. The villagers were dependent on the ocean for their livelihood and it was a daily part of their lives.<br>

                    &emsp;Alfred was the eldest of the three and was born into a family of fishermen. His father was a skilled fisherman and taught him everything he knew about the ocean and how to survive on it. From a young age, Alfred was fascinated by the ocean and would often spend his days exploring the tide pools and riding the waves.<br>

                    &emsp;Storm was the wild child of the group and was born into a family of sailors. His father was a skilled sailor and would often take Storm on his ship, teaching him how to navigate the ocean and how to handle a ship. Storm was always pushing the limits and loved to ride the biggest and most dangerous waves.<br>

                    &emsp;Maia was the youngest of the three and was born into a family of healers. Her mother was a skilled healer and would often take Maia with her to gather herbs and plants from the ocean. From a young age, Maia was fascinated by the ocean's plants and creatures and would often use her knowledge to heal the village's ailments.<br>

                    &emsp;As they grew older, the three of them became friends and started to explore the ocean together. They were all skilled surfers and loved to ride the waves together. They were also known for their bravery and would often put themselves in danger to save the village.<br>

                    &emsp;The three of them had a deep connection with the ocean and it was a part of their lives. They were determined to protect their village and the ocean, and they always acted in the best interest of their people."
                ?>
            </div>
        </div>
        <div class="closing">
            <div class="center">
                <div class="home">
                    <a href="/">
                        <button type="button" class="fill">home</button>
                    </a>
                </div>
            </div>
        </div>
    </body>
</html>
