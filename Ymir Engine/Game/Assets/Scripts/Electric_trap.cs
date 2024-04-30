using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Electric_trap : YmirComponent
{

    private float time = 0f;
    private bool activate = true;
    private bool hitPlayer = false;
    private Vector3 originalPosition;
    public float damage = 220;
    //public string toParticle = ""; electricidad
    //int aux = 0;

    public void Start()
    {
        originalPosition = gameObject.transform.globalPosition;
    }

    public void Update()
    {


        if (activate)
        {
            if (time < 3f)
            {
                gameObject.SetPosition(Vector3.negativeInfinity * Time.deltaTime * 1f);
                //InternalCalls.GetGameObjectByName(toParticle).SetActive(false);
            }
            else
            {
                activate = false;
                time = 0f;
            }
        }
        else
        {
            if (time < 3f)
            {
                gameObject.SetPosition(originalPosition);
                // InternalCalls.GetGameObjectByName(toParticle).SetActive(true);
                if (hitPlayer)
                {
                    activate = true;
                    hitPlayer = false;
                    time = 0f;
                    //Debug.Log("Hit Player");
                }
            }
            else
            {
                activate = true;
                time = 0f;
            }
        }

        time += Time.deltaTime;
        return;
    }

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player")
        {
            if (!activate)
            {
                other.GetComponent<Health>().TakeDmg(damage);
                hitPlayer = true;
                // Debug.Log("" + other.GetComponent<Health>().currentHealth);
                // Debug.Log("" + aux);
                // aux++;
            }
        }
    }

}